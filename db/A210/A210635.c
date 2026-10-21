/*
 * A210635  Array read by descending antidiagonals: a(n,w) = ((w - (n mod w) -
 *          1) + n) - (n mod w), n >= 0, w >= 1.
 * https://oeis.org/A210635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = value at position n read by descending antidiagonals of
 *            the transposed array defined by a(j,w) = ((w - (j mod w) - 1) +
 *            j) - (j mod w). The global index n is mapped to the pair (i,j)
 *            with i decreasing along each antidiagonal, then a(i,j) is
 *            computed as (j + (i+1) - 1 - 2*(j mod (i+1)))
 * Range:     u8 n <= 9395, u16 n <= 9395, u32 n <= 9395, u64 n <= 9395,
 *            u128 n <= 9395, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular(idx_t r) { return r * (r + 1) / 2; }

/* a(n) as described above */
seq_t a(idx_t n)
{
    /* find s such that triangular(s) <= n < triangular(s+1) */
    idx_t s = 0;
    while (triangular(s + 1) <= n) ++s;

    idx_t T = triangular(s);           /* sum of lengths of all previous antidiagonals */
    idx_t t = n - T;                    /* offset within the current antidiagonal */
    idx_t i = s - t;                    /* row index (0‑based) */
    idx_t j = t;                        /* column index (0‑based) */
    idx_t w = i + 1;                    /* width of the column */
    idx_t jmod = j % w;                 /* j modulo w */

    /* compute a(i,j) = ((w - jmod - 1) + j) - jmod = j + w - 1 - 2*jmod */
    return (seq_t)j + (seq_t)w - (seq_t)1 - (seq_t)2 * (seq_t)jmod;
}
