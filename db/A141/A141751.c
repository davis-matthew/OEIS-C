/*
 * A141751  Triangle, read by rows, where T(n,k) = [T(n-1,k-1)*T(n-1,k) +
 *          1]/T(n-2,k-1) for 0<k<n with T(n,n) = 1 for n>=0 and T(n,0) =
 *          Fibonacci(2*n-1) for n>=1.
 * https://oeis.org/A141751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows, with T(r,c) =
 *            Fibonacci(2*(r-c)-1) + c*Fibonacci(2*(r-c)) for 0 <= c < r, and
 *            T(r,r) = 1. The index n is flattened row‑wise. The conversion
 *            from a linear index n to (r,c) is t = floor((sqrt(8*n+1)+1)/2);
 *            // t = r+1 r = t-1; c = n - r*(r+1)/2;
 * Range:     u8 n <= 35, u16 n <= 90, u32 n <= 324, u64 n <= 1080,
 *            u128 n <= 1080, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* find row r and column c from the linear index n */
    long double tmp = sqrt((long double)8.0L * (long double)n + 1.0L);
    idx_t t = (idx_t)((tmp + 1.0L) / 2.0L);   /* t = r+1 */
    idx_t r = t - 1;
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t c = n - start_of_row;              /* 0 <= c <= r */

    /* special case: diagonal entry is 1 */
    if (c == r) {
        return (seq_t)1;
    }

    /* compute the value using the explicit formula */
    idx_t d = r - c;                     /* d >= 1 */
    seq_t f1 = fib(2 * d - 1);           /* Fibonacci(2*d-1) */
    seq_t f2 = fib(2 * d);               /* Fibonacci(2*d)   */
    seq_t res = f1 + (seq_t)c * f2;      /* f1 + c*f2         */

    return res;
}
