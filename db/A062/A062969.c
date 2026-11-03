/*
 * A062969  Numbers k such that abs(d(k)-k-1) is prime, where d(k) is the
 *          number of divisors of k.
 * https://oeis.org/A062969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062969 – Numbers k such that abs(d(k)-k-1) is prime.
   Memoised generation of the n‑th term (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"
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

/* Static cache of found terms */
static seq_t *cache      = NULL;   /* pointer to the array of terms          */
static size_t cache_cap  = 0;     /* allocated elements                     */
static size_t cache_size = 0;     /* number of valid terms in the cache    */
static idx_t  last_checked = 0;   /* last integer k that was examined      */

/* Grow the cache array to accommodate more terms.  */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that at least n terms are available in the cache. */
static void ensure_nterms(idx_t n)
{
    while (cache_size < (size_t)n) {
        ++last_checked;                      /* examine next integer k     */
        /* Count divisors of k (k fits in uint64_t). */
        uint64_t k      = (uint64_t)last_checked;
        uint64_t d      = num_divisors(k);
        uint64_t diff   = 0; /* abs(d - (k+1)) */
        if (d > k + 1)
            diff = d - (k + 1);
        else
            diff = (k + 1) - d;
        /* Test primality of the difference. */
        if (is_prime_big((seq_t)diff)) {
            if (cache_size >= cache_cap) grow_cache();
            cache[cache_size++] = (seq_t)k;
        }
    }
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of defined offset range      */
    ensure_nterms(n);
    return cache[(size_t)n - 1];
}
