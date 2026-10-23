/*
 * A192239  Coefficient of x in the reduction of the polynomial
 *          x(x+1)(x+2)...(x+n-1) by x^2 -> x+1.
 * https://oeis.org/A192239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=3; for n>3:
 *            a(n)=2*(n-1)*a(n-1)-(n^2-3*n+1)*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) : a(2) */
    seq_t a_nm1 = (seq_t)3;   /* a(n-1) : a(3) */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t mul1 = 2 * (seq_t)(i - 1);
        seq_t mul2 = (seq_t)(i * i - 3 * i + 1);
        a_n = mul1 * a_nm1 - mul2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
