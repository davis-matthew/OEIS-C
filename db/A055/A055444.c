/*
 * A055444  Base 4 distribution of first digit of mantissa following Benford's
 *          Law, to minimize chi-squared statistic.
 * https://oeis.org/A055444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A055444 – Base‑4 leading–digit distribution
 *
 *  For each n ≥ 1 the sequence gives the digit (1, 2 or 3) that,
 *  when appended to the existing list of n−1 digits, produces the
 *  smallest chi‑squared statistic relative to the Benford
 *  probabilities p₁ = log₂⁄log₄, p₂ = log(3/2)⁄log₄, p₃ = log(4/3)⁄log₄.
 *
 *  The implementation follows the greedy construction described on the
 *  OEIS page: at step n choose the digit that minimises
 *
 *      χ² = Σᵢ ((countᵢ + δᵢ) – n·pᵢ)^2 / (n·pᵢ)
 *
 *  where δᵢ is 1 if i is the candidate digit and 0 otherwise.
 *
 *  The algorithm keeps a cache of the sequence terms and their
 *  cumulative counts, so that any call to a(n) runs in O(1) amortised
 *  time after the necessary terms have been generated.
 */

#include <stdlib.h>
#include <math.h>

/* Benford probabilities for base‑4 leading digits */
static const double p[3] = {
    log(2.0)      / log(4.0),   /* digit 1 */
    log(1.5)      / log(4.0),   /* digit 2 */
    log(4.0/3.0)  / log(4.0)    /* digit 3 */
};

/* Cache of already computed digits (0‑based array) */
static seq_t *cache = NULL;
/* Number of cached terms */
static size_t cache_len = 0;
/* Current allocation size of cache */
static size_t cache_cap = 0;
/* Cumulative counts of digits 1,2,3 (index 0→1, 1→2, 2→3) */
static int digit_cnt[3] = {0, 0, 0};

/* Expand the cache so that it contains at least n terms */
static void expand_to(idx_t n)
{
    if ((size_t)n <= cache_len) return;          /* already available */

    /* Ensure sufficient capacity */
    size_t target = (size_t)n;
    if (cache_cap < target) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < target) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failure */
        cache = tmp;
        cache_cap = newcap;
    }

    /* Generate terms cache_len … target‑1 */
    for (size_t i = cache_len; i < target; ++i) {
        /* Index of the term in the sequence (1‑based) */
        idx_t m = (idx_t)(i + 1);
        int best_digit = 1;
        double best_chi  = 1e300;
        const double eps = 1e-12;

        /* Evaluate each possible digit */
        for (int d = 0; d < 3; ++d) {
            int new_cnt = digit_cnt[d] + 1;
            double chi2 = 0.0;
            for (int k = 0; k < 3; ++k) {
                int cur = (k == d) ? new_cnt : digit_cnt[k];
                double exp = (double)m * p[k];
                double diff = cur - exp;
                chi2 += diff * diff / exp;
            }
            if (chi2 < best_chi - eps ||
                (fabs(chi2 - best_chi) <= eps && (d + 1) < best_digit)) {
                best_chi   = chi2;
                best_digit = d + 1;   /* convert to 1‑based digit */
            }
        }

        cache[i] = (seq_t)best_digit;          /* store the chosen digit */
        digit_cnt[best_digit - 1]++;            /* update counts */
    }

    cache_len = target;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    expand_to(n);
    return cache[n - 1];
}
