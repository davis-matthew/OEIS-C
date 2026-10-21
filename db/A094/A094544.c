/*
 * A094544  Triangle of a(n,m) = number of m-member minimal T_0-covers of an
 *          n-set (n >= 0, 0<= m <=n).
 * https://oeis.org/A094544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(row, col) = row! / col! * C(2^col - col - 1, row - col)
 *            Enumerated row‑major: index n -> (row, col), result is the
 *            triangular entry a(row, col)
 * Range:     u8 n <= 23, u16 n <= 39, u32 n <= 70, u64 n <= 126,
 *            u128 n <= 240, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"

/* Compute the triangular entry a(row, col) from the index n.  */
seq_t a(idx_t n)
{
    /* Determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    long double nd = (long double)n;
    idx_t r = (idx_t)((-1.0L + sqrtl(1.0L + 8.0L * nd)) / 2.0L);
    idx_t prev = r * (r + 1) / 2;            /* n of row r‑1 */
    idx_t col = n - prev;                    /* column within the row */

    /* Compute the factorial ratio: row! / col! = ∏_{i=col+1}^{row} i. */
    seq_t ratio = 1;
    for (idx_t i = col + 1; i <= r; ++i)
        ratio *= (seq_t)i;

    /* Compute the binomial coefficient C(2^col - col - 1, row - col). */
    seq_t pow2m = pow2(col);
    seq_t top = pow2m - ((seq_t)(col + 1));
    idx_t k = r - col;

    if ((seq_t)k > top) /* binomial is zero if top < k. */
        return 0;

    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = top - (seq_t)(k - i);
        binom *= numer;
        binom /= (seq_t)i;  /* exact division */
    }

    return ratio * binom;
}
