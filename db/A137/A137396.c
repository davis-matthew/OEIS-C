/*
 * A137396  Triangle read by rows: row n gives the coefficients in the
 *          expansion of the chromatic polynomial of the n-cycle graphs.
 * https://oeis.org/A137396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 37, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the coefficient of x^k in the chromatic polynomial of the n‑cycle graph,
   read row by row.  For row r (r≥2) and 0≤k≤r:
     T(r,0) = 0
     T(r,1) = (r‑1)·(‑1)^(r‑1)
     T(r,k) = (‑1)^(r‑k)·binomial(r,k)   for k≥2
   The global index n satisfies cum_rows(r‑1)<n≤cum_rows(r), where
   cum_rows(r)= (r^2+3r‑2)/2 and cum_rows(0)=0. */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

/* cumulative number of terms up to row r (r≥0) */
static inline idx_t cum_rows(idx_t r)
{
    if (r == 0) return 0;
    return (r * r + 3 * r - 2) / 2;
}

seq_t a(idx_t n)
{
    /* determine row r and column k from the global index n */
    double d = sqrt((double)(8.0 * (double)n + 17.0));
    idx_t r = (idx_t)d;
    if (r < 1) r = 1;

    /* adjust r in case the estimate is slightly low */
    while (cum_rows(r) < n) ++r;
    while (r > 1 && cum_rows(r - 1) >= n) --r;

    idx_t prev = cum_rows(r - 1);
    idx_t k = n - prev - 1;            /* 0‑based index within the row */

    /* compute the triangle value */
    if (k == 0) return 0;

    if (k == 1) {
        seq_t val = (seq_t)(r - 1);
        if ((r - 1) & 1) val = -val;
        return val;
    }

    seq_t val = binomial(r, k);
    if ((r - k) & 1) val = -val;
    return val;
}
