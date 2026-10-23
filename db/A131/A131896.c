/*
 * A131896  A000012 * A131843.
 * https://oeis.org/A131896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r be the smallest integer with r*(r+1)/2 >= n, let c = n -
 *            r*(r-1)/2 (1 ≤ c ≤ r). Then a(n) = r^2 if c == 1 r + c - 1
 *            otherwise
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* find r approximately using the quadratic formula */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* correct r if the inequality is not satisfied */
    if ((long double)r * (r + 1) / 2.0L < (long double)n)
        ++r;

    /* compute position within the r‑th row */
    idx_t prevTri = r * (r - 1) / 2;   /* r*(r-1)/2 fits in 64‑bit */
    idx_t c = n - prevTri;

    /* return the corresponding entry of the triangle */
    if (c == 1) {
        seq_t sr = (seq_t)r;
        return sr * sr;                 /* r^2 */
    } else {
        return (seq_t)r + (seq_t)c - 1;  /* r + c – 1 */
    }
}
