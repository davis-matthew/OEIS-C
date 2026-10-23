/*
 * A180941  Numbers n such that n^2 - 2999n + 2248541 is semiprime.
 * https://oeis.org/A180941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th integer n such that n^2 – 2999*n + 2248541 is a
 *            semiprime
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Simple primality test for 64‑bit numbers, sufficient for the
   values encountered (0 < n,  n^2 ≤ 10^8 for n ≤ 10 000). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Test whether m is a semiprime (product of exactly two primes,
   counting multiplicity). */
static bool is_semiprime(uint64_t m)
{
    if (m < 4) return false;            /* smallest semiprime is 4 = 2*2 */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (!is_prime_uint64(p)) continue;
            uint64_t q = m / p;
            return q >= 2 && is_prime_uint64(q);
        }
    }
    return false;
}

/* Cache of already–computed terms.  Static storage is shared between
   successive calls and does not depend on call order. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t   next_test = 2;      /* start testing n=2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but harmless */

    /* Initialise cache if necessary. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        uint64_t cand = (uint64_t)next_test;
        /* Compute f(cand) = cand^2 – 2999*cand + 2248541 using 128-bit
           arithmetic to avoid overflow for larger n. */
        __int128 val128 = (__int128)cand * cand
                       - (__int128)2999 * cand
                       + 2248541LL;
        uint64_t val = (uint64_t)val128;

        if (is_semiprime(val)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)cand;
        }
        ++next_test;
    }

    return cache[n - 1];
}
