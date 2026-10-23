/*
 * A028892  a(n) = Fibonacci(n) - 2^(floor(n/2)).
 * https://oeis.org/A028892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Fibonacci(n) - 2^(floor(n/2))
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci number F(n) with F(0)=0, F(1)=1 */
    seq_t f0 = 0;          /* F(k-2) */
    seq_t f1 = 1;          /* F(k-1) */

    if (n != 0) {
        /* iterate from k = 2 up to n */
        for (idx_t k = 2; k <= n; ++k) {
            seq_t f2 = f0 + f1; /* F(k) = F(k-2) + F(k-1) */
            f0 = f1;
            f1 = f2;
        }
    }

    seq_t fn = (n == 0) ? f0 : f1;   /* F(n) */
    seq_t pow2_val = pow2(n >> 1);   /* 2^floor(n/2) */
    return fn - pow2_val;
}
