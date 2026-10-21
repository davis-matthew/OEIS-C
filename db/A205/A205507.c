/*
 * A205507  a(n) = Fibonacci(n) * A004018(n) for n>=1 with a(0)=1, where
 *          A004018(n) is the number of ways of writing n as a sum of 2
 *          squares.
 * https://oeis.org/A205507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A004018(n) where A004018(n) is the number
 *            of integer pairs (x, y) satisfying x^2 + y^2 = n (signs and
 *            order counted). This uses the prime‑factorisation formula r_2(n)
 *            = 4 * Π_{p≡1 (mod4)} (e_p + 1), provided no prime ≡3 (mod4)
 *            occurs to an odd exponent, otherwise r_2(n)=0
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 44, u64 n <= 89, u128 n <= 184,
 *            big any n
 * Helpers:   fib, sum_of_two_squares_cnt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/sum_of_two_squares_cnt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 by definition */

    seq_t fib_n = fib(n);              /* Fibonacci(n) from utils */
    seq_t rep   = sum_of_two_squares_cnt(n);

    return fib_n * rep;
}
