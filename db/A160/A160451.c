/*
 * A160451  a(n) = (4/3)*u*(u^3+6*u^2+8*u-3) where u=floor((3*n+5)/2).
 * https://oeis.org/A160451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/3)*u*(u^3+6*u^2+8*u-3) where u = floor((3*n+5)/2)
 * Range:     u8 none, u16 n <= 4, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* value undefined for offset 1, return 0 */
    seq_t n_seq = (seq_t)n;
    /* compute u = floor((3*n+5)/2) */
    seq_t u = ((seq_t)3 * n_seq + (seq_t)5) / (seq_t)2;
    /* t = u^3 + 6*u^2 + 8*u - 3 */
    seq_t t = u * u * u + (seq_t)6 * u * u + (seq_t)8 * u - (seq_t)3;
    /* numerator = 4*u*t */
    seq_t num = (seq_t)4 * u * t;
    /* final value = num / 3 (exact division) */
    return num / (seq_t)3;
}
