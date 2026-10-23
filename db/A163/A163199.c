/*
 * A163199  Sum of the cubes of the first n even-indexed Fibonacci numbers,
 *          minus 1.
 * https://oeis.org/A163199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/4)*(F(2n+1)^3 - 3*F(2n+1) - 2)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F(k) is the k-th Fibonacci number (F(0)=0, F(1)=1). */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the index 2*n+1 needed for F(2n+1). */
    idx_t idx = n * 2 + 1;
    seq_t f = fib(idx);           /* F(2n+1) */
    seq_t f3 = f * f * f;         /* f^3  */
    seq_t sub = f3 - 3 * f - 2;   /* f^3 - 3*f - 2 */
    return sub / 4;               /* divide by 4 truncating toward zero */
}
