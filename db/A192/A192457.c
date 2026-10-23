/*
 * A192457  Constant term of the reduction by x^2->x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A192457: constant term of the reduction by x^2->x+2 of 
 * p(n,x) defined recursively by p(0,x)=x, p(n,x)=(x+2n)*p(n-1,x). 
 *
 * The coefficients of the reduced polynomial satisfy the recurrence:
 *   b_0 = 0
 *   b_1 = 2
 *   b_{n+1} = (4n + 3) * b_n + 2 * (1 - n - 2n^2) * b_{n-1}
 *
 * where b_n is the constant term of the reduced p(n,x).  Equivalently,
 * b_n can be obtained iteratively using the linear system:
 *   a_0 = 1,  b_0 = 0
 *   a_n = (1 + 2n) * a_{n-1} + b_{n-1}
 *   b_n = 2 * (a_{n-1} + n * b_{n-1})
 *
 * The function a(idx_t n) returns b_n.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t a_prev = (seq_t)1;  /* coefficient of x for n=0 */
    seq_t b_prev = (seq_t)0;  /* constant term for n=0 */

    seq_t a_curr, b_curr;
    for (idx_t i = 1; i <= n; ++i) {
        a_curr = a_prev * (seq_t)(1 + 2 * i) + b_prev;
        b_curr = (seq_t)2 * (a_prev + (seq_t)i * b_prev);
        a_prev = a_curr;
        b_prev = b_curr;
    }
    return b_prev;
}
