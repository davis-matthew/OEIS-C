/*
 * A116562  Duplicate of A045925.
 * https://oeis.org/A116562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n * Fib(n) (with Fib(1)=1, Fib(2)=1)
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t f = fib(n);           /* Fib(n) in the same type as seq_t  */
    return (seq_t)n * f;        /* multiply using seq_t arithmetic   */
}
