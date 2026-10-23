/*
 * A143603  Triangle, read by rows, such that the g.f. of column k =
 *          G(x)^(2k+1) where G(x) = 1 + x*G(x)^3 is the g.f. of A001764
 *          (ternary trees).
 * https://oeis.org/A143603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle T(n,k) with T(n,k) = C(3n-k
 *            , n-k) * (2k+1)/(2n+1) (0 ≤ k ≤ n) The triangle is read by rows:
 *            first row n=0 has 1 element, second row n=1 has 2 elements,
 *            etc., with OEIS offset 1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 (offset 1) */
    if (n == 0) return 0;   /* Safety: should not happen */

    /* Find the row r such that (r+1)(r+2)/2 ≥ n > r(r+1)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) r++;

    idx_t prev = r * (r + 1) / 2;        /* number of terms before row r   */
    idx_t k   = n - prev - 1;           /* column index in row r (0‑based) */

    /* Compute T(r,k) = binom(3r-k , r-k) * (2k+1) / (2r+1) */
    idx_t num1 = 3 * r - k;              /* upper argument of binomial   */
    idx_t den1 = r - k;                  /* lower argument of binomial   */
    seq_t bin  = binom(num1, den1);      /* binomial coefficient        */

    seq_t num = bin * ((seq_t)(2 * k + 1));
    seq_t den = (seq_t)(2 * r + 1);

    return num / den;   /* division is exact by definition */
}
