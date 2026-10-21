/*
 * A205549  Symmetric matrix by antidiagonals: C(max(i+2,j+2),min(i+2,j+2)),
 *          i>=1, j>=1.
 * https://oeis.org/A205549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C( max(i+2, j+2), min(i+2, j+2) ) * where (i,j) are the
 *            antidiagonal coordinates of the n‑th element * in the infinite
 *            matrix indexed by i,j >= 1
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

/* Compute a(n) for the OEIS sequence A205549.
 * idx_t is an unsigned 64‑bit integer; seq_t is an unsigned integer
 * type (possibly arbitrary‑precision).  The implementation follows
 * the antidiagonal enumeration used in the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – n >= 1 for all valid calls */
        return 0;

    /* Find the diagonal sum s = i + j such that
     * (s-1)*s/2 >= n  (count of elements up to this diagonal).   */
    idx_t s = (idx_t)((sqrt((double)1.0 + 8.0 * (double)n) + 1.0) / 2.0);

    /* Correct s upwards if the inequality is not satisfied.       */
    while (( (__uint128_t)s * (s - 1) ) / 2 < n)
        ++s;

    /* Elements before this diagonal: (s-2)*(s-1)/2.                */
    __uint128_t prev = ( (__uint128_t)(s - 2) * (s - 1) ) / 2;

    /* Position inside the diagonal (1‑based).                       */
    idx_t r = n - (idx_t)prev;

    /* Map position r to coordinates (i,j).                       */
    idx_t i = r;         /* i runs from 1 to s-1   */
    idx_t j = s - r;     /* j = (i+j) - i         */

    /* Assemble parameters for the binomial coefficient.           */
    idx_t n_binom = (i > j) ? i + 2 : j + 2;
    idx_t k_binom = (i > j) ? j + 2 : i + 2;

    return binom(n_binom, k_binom);
}
