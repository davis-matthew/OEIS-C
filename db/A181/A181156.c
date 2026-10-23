/*
 * A181156  Odd Fibonacci numbers F which have a proper Fibonacci divisor G
 *          such that F/G is a Lucas number or a product of Lucas numbers.
 * https://oeis.org/A181156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A181156: odd Fibonacci numbers F that have a proper Fibonacci divisor G
 * such that F/G is a Lucas number or a product of Lucas numbers.
 * For n >= 1 the index of the Fibonacci number is
 *   index(n) = 4 + 6 * ((n - 1) / 2)  if n is odd
 *   index(n) = 8 + 6 * ((n - 2) / 2)  if n is even
 * (index 2, i.e. F_2 = 1, is excluded because it has no proper Fibonacci divisor).
 * The nth sequence value is fib(index(n)).
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n = 0, return 0 */
    idx_t idx;
    if (n & 1U)   /* odd n */
        idx = 4 + 6 * ((n - 1) / 2);
    else          /* even n */
        idx = 8 + 6 * ((n - 2) / 2);
    return fib(idx);
}
