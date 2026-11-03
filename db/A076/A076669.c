/*
 * A076669  Primes of the form n^2*totient(n)+1 (or A053191(n) + 1).
 * https://oeis.org/A076669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime of the form n^2·φ(n)+1 (checked for n =
 *            1,2,3, …)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u64, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/totient_seq.h"
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

static seq_t *cache = NULL;        /* stored prime values */
static size_t cache_len = 0;       /* number of stored primes */
static size_t cache_cap = 0;       /* current capacity of cache array */
static idx_t next_n = 1;           /* next n to test */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete [] cache;
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    free(cache);
#endif
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1; value undefined for 0 */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute terms until we have at least n primes */
    while (cache_len < n) {
        seq_t nn = (seq_t)next_n;
        seq_t phi = totient_seq(nn);
        seq_t val = nn * nn * phi + 1;

        if (is_prime_big(val)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_n;
    }

    return cache[n - 1];
}
