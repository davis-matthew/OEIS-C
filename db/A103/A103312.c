/*
 * A103312  A transform of the Jacobsthal numbers.
 * https://oeis.org/A103312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 41, i64 n <= 82, i128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=a(2)=a(3)=1;
   for n>=4, a(n)=3*a(n-1)-3*a(n-2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 3) return 1;     /* covers n==1,2,3 */

    seq_t a_nm2 = 1;          /* a(n-2) */
    seq_t a_nm1 = 1;          /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = (seq_t)3 * a_nm1 - (seq_t)3 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }
    return a_nm1;
}
