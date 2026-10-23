/*
 * A178982  Partial sums of floor(Fibonacci(n)/2).
 * https://oeis.org/A178982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fibonacci(n+2)+floor(n/3)-n-1)/2
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    // Fibonacci of n+2
    seq_t fn2 = fib(n + 2);

    // floor(n/3) using integer division
    seq_t ndiv3 = (seq_t)n / (seq_t)3;

    // sum: F(n+2) + floor(n/3) - n - 1
    seq_t sum = fn2 + ndiv3 - (seq_t)n - (seq_t)1;

    // divide by 2 (exact division)
    return sum >> 1;
}
