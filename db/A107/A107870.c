/*
 * A107870  Triangle, read by rows, where T(n,k) =
 *          C(n*(n-1)/2-k*(k-1)/2+n-k+2, n-k).
 * https://oeis.org/A107870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = row_of_index(n), k = n - r*(r+1)/2,
 *            T(n,k) = C( n*(n-1)/2 - k*(k-1)/2 + n - k + 2 , n-k )
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrtl(8.0L * n + 1) - 1) / 2);
    /* correct possible rounding errors */
    while (r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    idx_t k = n - r * (r + 1) / 2;  /* column index in row r */

    /* compute the binomial parameters */
    seq_t rn = (seq_t)r * (r - 1) / 2;          /* r*(r-1)/2 */
    seq_t kn = (seq_t)k * (k - 1) / 2;          /* k*(k-1)/2 */
    seq_t N  = rn - kn + r - k + 2;             /* numerator */
    seq_t K  = r - k;                           /* denominator */

    return binom((idx_t)N, (idx_t)K);
}
