/*
 * A221055  Primes of the form k*(k+1)*(k+2)/6+2 (i.e., two more than a
 *          tetrahedral number).
 * https://oeis.org/A221055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form k*(k+1)*(k+2)/6 + 2 (i.e. T_k + 2 where
 *            T_k is the k-th tetrahedral number)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   binomial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#ifdef __cplusplus
#include <vector>
#endif

/* static state */
static idx_t    next_k = 0;  /* smallest k not yet tested (k starts at 0) */

#ifdef __cplusplus
static std::vector<seq_t> prime_cache;
#else
static seq_t *prime_cache = NULL;
static size_t prime_len   = 0;   /* number of primes stored */
static size_t prime_cap   = 0;   /* capacity of the array */
#endif

/* Append a prime to the cache (grow array if needed) */
static void
append_prime(seq_t p)
{
#ifdef __cplusplus
    prime_cache.push_back(p);
#else
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap << 1;
        seq_t *tmp = realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        prime_cache = tmp;
        prime_cap = new_cap;
    }
    prime_cache[prime_len++] = p;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1 */

    /* early return if required prime already cached */
#ifdef __cplusplus
    if (n <= prime_cache.size())
        return prime_cache[n - 1];
#else
    if (n <= prime_len)
        return prime_cache[n - 1];
#endif

    /* generate primes until the n-th one is found */
    for (;;) {
        seq_t term = binomial(next_k + 2, 3) + 2;   /* T_k + 2 */
        if (is_prime_big(term))
            append_prime(term);

        next_k++;   /* advance to next k regardless of primality */

#ifdef __cplusplus
        if (n <= prime_cache.size())
            return prime_cache[n - 1];
#else
        if (n <= prime_len)
            return prime_cache[n - 1];
#endif
    }
}
