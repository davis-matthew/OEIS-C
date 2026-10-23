/*
 * A140284  A weighted binomial triangle with smooth weighting function:
 *          f(n,d) = Floor[1 + d*Sech[d/2 - n]].
 * https://oeis.org/A140284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   a(n) = weighted binomial triangle entry * a(n) = f(k,d) *
 *            Binomial(d,k), with * f(k,d) = Floor[1 + d * Sech[d/2 - k]] * d
 *            is the row index (starting at 0), k is the column index * (0 ≤ k
 *            ≤ d). The sequence is listed row‑wise, offset 1
 * Range:     u8 n <= 39, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Compute the number of terms up to and including row d (row indices start at 0). */
static inline unsigned __int128 triangular(idx_t d)
{
    return (unsigned __int128)(d + 1) * (d + 2) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, offset starts at 1 */

    /* Find row d such that triangular(d) ≥ n. */
    idx_t low = 0, high = 1;
    while (triangular(high) < (unsigned __int128)n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* Number of elements before row d. */
    unsigned __int128 prev_terms = (unsigned __int128)d * (d + 1) / 2;
    idx_t k = n - (idx_t)prev_terms - 1;   /* 0‑based column index */

    /* Endpoints are always 1. */
    if (k == 0 || k == d) return (seq_t)1;

    /* Compute weight f = floor(1 + d*Sech[d/2 - k]). */
    double x      = (double)d / 2.0 - (double)k;
    double sech   = 1.0 / cosh(x);
    double fval   = floor(1.0 + (double)d * sech);
    seq_t f       = (seq_t)(uint64_t)fval;   /* f fits in 64 bits for our ranges */

    /* Binomial coefficient Binomial(d,k) via provided helper. */
    seq_t bin = binom(d, k);

    return f * bin;
}
