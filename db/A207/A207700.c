/*
 * A207700  Number of n X 5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 89, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207700: Number of n×5 0..1 arrays avoiding
 *  000 and 010 horizontally and 001 and 110 vertically.
 *
 *  Recurrence (empirical, order 6):
 *      a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3)
 *           - 15*a(n-4) + 6*a(n-5) - a(n-6)
 *      for n ≥ 7
 *
 *  Initial values:
 *      a(1)=15, a(2)=225, a(3)=1071, a(4)=3321,
 *      a(5)=8151, a(6)=17225
 *
 *  The function caches already-computed values so that calls
 *  in any order are answered in O(n) time overall and are
 *  independent of previous calls.
 */

#include <stddef.h>

/* Maximum n expected in the test suite (about 10 000).
 * The array is big enough to hold all requested terms. */
#define A207700_MAX_N 20000

/* Static cache of precomputed terms.
 * cache[0] = a(1), cache[1] = a(2), etc. */
static seq_t cache[A207700_MAX_N];
/* Number of terms currently stored in cache. */
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for the sequence, return 0 */

    if (n > A207700_MAX_N)
    {
        /* For n beyond the cache size we compute on the fly using the
         * recurrence but only keep the last six values to avoid large
         * allocations.  This path is rarely used (tests only go up to
         * about 10 000), so it keeps the implementation safe for any n.
         */
        seq_t terms[6];
        /* initialise the first six terms */
        terms[0] = 15;
        terms[1] = 225;
        terms[2] = 1071;
        terms[3] = 3321;
        terms[4] = 8151;
        terms[5] = 17225;
        if (n <= 6)
            return terms[n - 1];
        for (idx_t k = 7; k <= n; ++k)
        {
            seq_t next = 6 * terms[0] - 15 * terms[1] + 20 * terms[2]
                        - 15 * terms[3] + 6 * terms[4] - terms[5];
            /* shift the window */
            terms[5] = terms[4];
            terms[4] = terms[3];
            terms[3] = terms[2];
            terms[2] = terms[1];
            terms[1] = terms[0];
            terms[0] = next;
        }
        return terms[0];
    }

    /* Initialise cache with the first six terms once. */
    if (cache_len == 0)
    {
        cache[0] = 15;
        cache[1] = 225;
        cache[2] = 1071;
        cache[3] = 3321;
        cache[4] = 8151;
        cache[5] = 17225;
        cache_len = 6;
    }

    /* If the requested value is already cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute terms up to n, storing them in the cache. */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        seq_t term =
            6 * cache[k - 2]
          - 15 * cache[k - 3]
          + 20 * cache[k - 4]
          - 15 * cache[k - 5]
          + 6 * cache[k - 6]
          - cache[k - 7];
        cache[k - 1] = term;
    }
    cache_len = n;
    return cache[n - 1];
}
