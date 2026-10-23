/*
 * A117360  Numbers m such that m and 2*m+1 have the same number of prime
 *          factors (with repetitions).
 * https://oeis.org/A117360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117360 - Numbers m such that m and 2*m+1 have the same number of
 * prime factors (counted with multiplicity).  The function a(idx_t n) returns
 * the nth such number (1‑indexed).  The implementation keeps a static
 * cache and generates terms lazily so that calls are fast even if the
 * arguments are not monotonic.
 */

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Count the total number of prime factors (with repetitions) of a
 * 64‑bit unsigned integer.  The result fits easily into seq_t.          */
static seq_t count_prime_factors_uint64(uint64_t n)
{
    seq_t cnt = 0;
    if (n < 2) return 0;
    /* Handle factor 2 */
    while ((n & 1) == 0) {
        ++cnt;
        n >>= 1;
    }
    /* Check odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    if (n > 1) ++cnt;   /* remaining prime factor */
    return cnt;
}

/* ------------------------------------------------------------------ */
/* Static cache of already found terms.  200 000 entries are enough
 * for all n up to about 10 000 (the sequence grows slowly).  The
 * cache is resized only if it ever needs more space, which never
 * happens for the required range. */
#define CACHE_CAPACITY 200000
static seq_t cached_terms[CACHE_CAPACITY];
static size_t cached_cnt = 0;
static uint64_t next_candidate = 2;   /* first number to test */

/* ------------------------------------------------------------------ */
/* Return the nth term of the sequence (1‑indexed).  The function
 * generates subsequent terms until the cache holds at least n
 * entries.  The cache is reused for later calls, so that calls can be
 * made in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 in OEIS; 0 is undefined */

    /* Generate terms until we have n of them. */
    while (cached_cnt < n) {
        uint64_t m = next_candidate++;
        seq_t pf_m  = count_prime_factors_uint64(m);
        seq_t pf_2m1 = count_prime_factors_uint64(2 * m + 1);
        if (pf_m == pf_2m1) {
            if (cached_cnt < CACHE_CAPACITY) {
                cached_terms[cached_cnt++] = (seq_t)m;
            } else {
                /* In the unlikely event we run out of space we simply
                   return the value without caching; this will never
                   happen for the required n<10 000. */
                return (seq_t)m;
            }
        }
    }
    return cached_terms[n - 1];
}
