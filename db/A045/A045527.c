/*
 * A045527  Numbers k such that k! has initial digit '8'.
 * https://oeis.org/A045527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that k! has leading digit 8. *
 *            The function computes the sequence lazily and stores all
 *            previously * found terms in a static cache
 * Range:     u8 n <= 16, u16 n <= 3357, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

static seq_t *cache = NULL;              /* cache of the computed terms */
static size_t cache_len = 0;             /* number of terms stored */
static size_t cache_cap = 0;             /* allocated capacity of cache */
static uint64_t current_k = 0;           /* last k processed (starting from 0) */
static double log_sum = 0.0;             /* sum of log10(k!) for current_k */

/* pre‑computed constants for the interval [log10(8), log10(9)) */
static const double LOG10_8 = 0.9030899870219337; /* log10(8)   */
static const double LOG10_9 = 0.9542425094393249; /* log10(9)   */

static void ensure_cache(idx_t n)
{
    /* Grow the cache until we have at least n terms available */
    while (cache_len < (size_t)n)
    {
        /* Move to the next k and update the cumulative log10 factorial */
        ++current_k;
        log_sum += log10((double)current_k);

        /* Fractional part of the log10(k!) */
        double frac = log_sum - floor(log_sum);

        /* Check whether the leading digit is 8 */
        if (frac >= LOG10_8 && frac < LOG10_9)
        {
            /* Append current_k to the cache */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but return 0 as sentinel */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
