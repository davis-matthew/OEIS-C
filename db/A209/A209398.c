/*
 * A209398  Number of subsets of {1,...,n} containing two elements whose
 *          difference is 2.
 * https://oeis.org/A209398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - F(2+floor(n/2)) * F(2+floor((n+1)/2)) (where F(k)
 *            is the k-th Fibonacci number, F(1)=1, F(2)=1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t two_n = pow2(n);

    /* Fibonacci arguments */
    idx_t f1_idx = 2 + n / 2;
    idx_t f2_idx = 2 + (n + 1) / 2;

    /* Fibonacci values */
    seq_t f1 = fib(f1_idx);
    seq_t f2 = fib(f2_idx);

    /* a(n) = two_n - f1 * f2 */
    return two_n - f1 * f2;
}
