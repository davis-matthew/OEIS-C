/*
 * A213172  Floor of the Euclidean distance of a point on the (1, 2, 3; 4, 5,
 *          6) 3D walk.
 * https://oeis.org/A213172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( x_n^2 + y_n^2 + z_n^2 ) ) where x_n =
 *            sum_{k=1..n, k mod 3 = 1} k = cx*(3*cx-1)/2 y_n = sum_{k=1..n, k
 *            mod 3 = 2} k = cy*(3*cy+1)/2 z_n = sum_{k=1..n, k mod 3 = 0} k =
 *            3*cz*(cz+1)/2 with cx = n/3 + (n%3>=1) cy = n/3 + (n%3>=2) cz =
 *            n/3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t m = n / 3;
    idx_t r = n % 3;

    idx_t cx = m + (r >= 1 ? 1 : 0);
    idx_t cy = m + (r >= 2 ? 1 : 0);
    idx_t cz = m;

    seq_t cx_s = (seq_t)cx;
    seq_t cy_s = (seq_t)cy;
    seq_t cz_s = (seq_t)cz;

    /* compute coordinates */
    seq_t x = cx_s * ((seq_t)3 * cx_s - (seq_t)1) / (seq_t)2;
    seq_t y = cy_s * ((seq_t)3 * cy_s + (seq_t)1) / (seq_t)2;
    seq_t z = (seq_t)3 * cz_s * (cz_s + (seq_t)1) / (seq_t)2;

    /* squared distance */
    seq_t d2 = x * x + y * y + z * z;

    /* floor of Euclidean distance */
    return sqrt_floor(d2);
}
