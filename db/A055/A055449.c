/*
 * A055449  Base 9 distribution of first digit of mantissa following Benford's
 *          Law, to minimize chi-squared statistic.
 * https://oeis.org/A055449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is chosen greedily – at each step the digit d ∈ {1,…,8}
 *            that minimises the chi‑squared statistic computed from the
 *            Benford probabilities of base 9 is appended to the current
 *            cumulative leading‑digit counts
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* Cached terms (1‑based indices are stored at cache[n‑1]) */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Current counts of each leading digit (1‑8) */
static idx_t digit_counts[8] = {0};

/* Benford probabilities for base 9, scaled to double */
static double benford_prob[8] = {0.0};

/* Initialise benford_prob exactly once */
static void init_benford(void)
{
    static int done = 0;
    if (done) return;
    double log9 = log(9.0);
    for (int d = 1; d <= 8; ++d)
        benford_prob[d-1] = (log((double)(d+1)) - log((double)d)) / log9;
    done = 1;
}

seq_t a(idx_t n)
{
    /* [OEIS offset] a(1) is the first term.  Values for n<1 are
       undefined for this sequence; return 0 to satisfy C linkage. */
    if (n == 0) return 0;

    init_benford();

    /* Grow cache if necessary */
    if ((idx_t)n > (idx_t)cache_len)
    {
        idx_t new_len = n;                     /* new length required */
        seq_t *tmp = (seq_t *)realloc(
            cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failure */
        cache = tmp;

        /* Generate missing terms up to new_len-1 (0‑based) */
        for (idx_t idx = (idx_t)cache_len; idx < new_len; ++idx)
        {
            double cur_n = (double)(idx + 1);
            int best_digit = 0;
            double best_chi = -1.0;            /* negative value forces update */

            /* Try each candidate digit */
            for (int d = 0; d < 8; ++d)
            {
                double chi = 0.0;
                for (int k = 0; k < 8; ++k)
                {
                    idx_t cnt = digit_counts[k];
                    if (k == d) ++cnt;         /* candidate addition */
                    double expected = cur_n * benford_prob[k];
                    double diff = (double)cnt - expected;
                    chi += (diff * diff) / expected;
                }
                if (best_chi < 0.0 || chi < best_chi)
                {
                    best_chi  = chi;
                    best_digit = d;
                }
            }

            /* Store term (1‑based digit) and update counts */
            cache[idx] = (seq_t)(best_digit + 1);
            ++digit_counts[best_digit];
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
