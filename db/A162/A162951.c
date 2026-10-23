/*
 * A162951  a(1)=0. a(n) is the smallest integer > a(n-1) such that both a(n)
 *          and a(n)-a(n-1) have the same number of 1's when written in binary
 *          as n has when it is written in binary.
 * https://oeis.org/A162951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: Computes A162951, searching for each term sequentially.
   a(1) = 0.
   For n > 1, a(n) is the smallest integer > a(n-1) such that
   popcount(a(n)) = popcount(a(n)-a(n-1)) = popcount(n).
   The sequence has only 7 defined terms; for n > 7 the function
   returns 0. */

#include <stdint.h>
#include <stdlib.h>

/* Count 1-bits in an arbitrary-width unsigned integer. */
static inline int popcount_seq(seq_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;          /* clear lowest set bit */
        ++c;
    }
    return c;
}

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of cache array */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against 0 */

    /* Ensure cache can hold n terms. */
    if (cache_len < n) {
        size_t need = n;
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap < need) new_cap <<= 1;

        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        if (cache == NULL) return 0;  /* allocation failure */

        /* Initialize new slots to 0. */
        for (size_t i = cache_len; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;

        /* Compute missing terms up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {                /* a(1) = 0 */
                cache[0] = 0;
                continue;
            }
            seq_t prev = cache[i - 2];
            int p = popcount_seq((seq_t)i);
            seq_t cand = prev + 1;

            /* Search for the smallest candidate.  Limit trials
               to avoid infinite loop beyond the defined length. */
            const idx_t MAX_TRIAL = 1000000;   /* arbitrary safe bound */
            idx_t trials = 0;
            while (1) {
                if (popcount_seq(cand) == p &&
                    popcount_seq(cand - prev) == p)
                    break;                 /* found valid term */
                ++cand;
                ++trials;
                if (trials > MAX_TRIAL) {
                    /* No valid term found – sequence ends. */
                    for (idx_t j = i; j <= n; ++j)
                        cache[j - 1] = 0;
                    cache_len = n;
                    return 0;
                }
            }
            cache[i - 1] = cand;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
