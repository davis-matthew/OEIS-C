/*
 * A077501  a(n) = smallest prime greater than a(n-1) and beginning with n.
 * https://oeis.org/A077501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime > a(n-1) that starts with decimal digits
 *            of n
 * Range:     u8 n <= 19, u16 n <= 654, u32 n <= 47, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   ensure_cache, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses simple incremental search and memoisation.
   Because the prime with a given prefix is usually close to the prefix,
   the search space for each n is small.  The code works for both
   fixed‑size and arbitrary‑precision builds. */

#include <stdbool.h>
#include <stddef.h>

/* Utilities required by the implementation */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */
#include "../utils/ensure_cache.h"   /* void ensure_cache(idx_t n, seq_t **, size_t *); */

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Find the smallest prime > prev that starts with decimal prefix 'pref' */
static seq_t next_prime_with_prefix(idx_t pref, seq_t prev)
{
    /* Search increasing lengths of the number of digits appended after the prefix */
    for (idx_t len = 0; /* empty */; ++len) {
        seq_t mult   = (seq_t)pref * pow10(len);          /* pref * 10^len           */
        seq_t block  = pow10(len) - (seq_t)1;             /* 10^len - 1               */
        seq_t block_max = mult + block;                   /* maximum number with this prefix */

        if (prev < block_max) {                           /* there is a candidate in this block */
            /* first candidate in this block that is > prev */
            seq_t start = mult;
            if (prev >= mult)
                start = prev + (seq_t)1;                  /* ensure strictly greater */

            /* iterate through the block to find a prime */
            for (seq_t cand = start; cand <= block_max; cand = cand + (seq_t)1) {
                if (is_prime_big(cand))
                    return cand;
            }
        }
        /* No prime in this block, try a longer representation */
    }
    /* Theoretically unreachable: by Dirichlet's theorem there will be a prime
       for some len. */
    return (seq_t)0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for offset 0 */

    /* Ensure cache can hold indices up to n */
    if (n > cache_cap) {
        ensure_cache(n, &cache, &cache_cap);   /* may allocate more, but at least n+1 size */
        /* initialise new entries to zero */
        for (idx_t i = cache_cap; i <= n; ++i) cache[i] = (seq_t)0;
        cache_cap = n;
    }

    /* Compute all missing terms up to n (memoised) */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already computed */

        seq_t prev = (i > 1) ? cache[i - 1] : (seq_t)0;
        cache[i] = next_prime_with_prefix(i, prev);
    }

    return cache[n];
}
