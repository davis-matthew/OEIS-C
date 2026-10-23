/*
 * A104559  Triangle, read by rows, of the number of left factors of peakless
 *          Motzkin paths of length n having k number of U's and D's (i.e.,
 *          number of paths from (0,0) to the line x=n, consisting of steps
 *          U=(1,1), H=(1,0), D=(1,1), that never go below the x-axis and a U
 *          step is never followed by a D step).
 * https://oeis.org/A104559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = row_of(n) k = n - r*(r+1)/2 and T(r, k)
 *            = C(r - floor(k/2), floor((k+1)/2)) * C(r - floor((k+1)/2),
 *            floor(k/2))
 * Range:     u8 n <= 48, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   binom_small, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A104559 */
#include "../utils/binom_small.h"
#include "../utils/row_of.h"

seq_t a(idx_t n) {
    /* Determine which row the index n falls in */
    idx_t r = row_of(n);
    /* Position within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute floor(k/2) and floor((k+1)/2) */
    idx_t u = k / 2;          /* floor(k/2)  */
    idx_t v = (k + 1) / 2;    /* floor((k+1)/2) */

    /* Compute the binomial products as per the formula */
    seq_t t1 = binom_small(r - u, v);
    seq_t t2 = binom_small(r - v, u);

    return t1 * t2;
}
