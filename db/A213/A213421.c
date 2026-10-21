/*
 * A213421  Real part of Q^n, Q being the quaternion 2+i+j+k.
 * https://oeis.org/A213421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2), a(0)=1, a(1)=2
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 46, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    idx_t i;
    seq_t a_nm2 = (seq_t)1;      /* a(n-2)   */
    seq_t a_nm1 = (seq_t)2;      /* a(n-1)   */
    seq_t cur  = a_nm1;          /* a(n)     */

    for (i = 2; i <= n; ++i)
    {
        cur = (seq_t)4 * a_nm1 - (seq_t)7 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
