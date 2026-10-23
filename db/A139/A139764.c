/*
 * A139764  Smallest term in Zeckendorf representation of n.
 * https://oeis.org/A139764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n if n is a Fibonacci number (1,2,3,5,…), * otherwise
 *            a(n) = a( n - (largest Fibonacci number < n) )
 * Range:     u8 n <= 376, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   largest_fib_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_fib_le.h"

seq_t a (idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined for 0, but safe */
    seq_t x = (seq_t)n;
    while (1) {
        seq_t f = largest_fib_le(x);
        if (f == x) return x;             /* x itself is Fibonacci */
        x = x - f;                        /* otherwise subtract the largest one */
    }
}
