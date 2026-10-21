/*
 * A202100  Square array read by antidiagonals: T(n,k) = Number of (n+2) X
 *          (k+2) binary arrays avoiding patterns 001 and 011 in rows and
 *          columns.
 * https://oeis.org/A202100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(i,j) along its
 *            antidiagonals. Each entry of the array is computed by T(i,j) =
 *            C(alpha+E,E) * C(alpha+O,O) * C(beta+E,E) * C(beta+O,O) where
 *            alpha = ceil((i+2)/2), beta = floor((i+2)/2), E = ceil((j+2)/2),
 *            O = floor((j+2)/2). The sequence starts at a(...
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 125, u64 n <= 511,
 *            u128 n <= 544, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial coefficient helper is provided by the build. */
#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t ceil_div2(idx_t x) { return (x + 1) >> 1; }
static inline idx_t floor_div2(idx_t x) { return x >> 1; }

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal (sum s = i+j) that contains the n‑th term.  */
    idx_t s = 2;                    /* minimal sum leading to first entry */
    while ((s - 1) * s / 2 < n)     /* (s-1)*s/2 = total terms up to sum s */
        ++s;

    idx_t prev = (s - 2) * (s - 1) / 2;     /* terms before this diagonal */
    idx_t pos  = n - prev - 1;              /* 0‑based offset within diagonal */
    idx_t t    = s - 1;                     /* number of elements in this diagonal */

    /* Determine (i,j) from diagonal position.  */
    idx_t i = t - pos;       /* row index (starts at 1) */
    idx_t j = 1 + pos;       /* column index (starts at 1) */

    /* Compute the parameters needed for the binomial products.  */
    idx_t alpha = ceil_div2(i + 2);
    idx_t beta  = floor_div2(i + 2);
    idx_t E     = ceil_div2(j + 2);
    idx_t O     = floor_div2(j + 2);

    /* Evaluate the four binomial factors.  */
    seq_t b1 = binomial(alpha + E, E);
    seq_t b2 = binomial(alpha + O, O);
    seq_t b3 = binomial(beta  + E, E);
    seq_t b4 = binomial(beta  + O, O);

    return b1 * b2 * b3 * b4;
}
