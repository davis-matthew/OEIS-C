/*
 * A161762  Sum of all numbers from and including Fibonacci(n-1) to and
 *          including Fibonacci(n).
 * https://oeis.org/A161762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=Fib(n-1)}^{Fib(n)} k = (Fib(n-1)+Fib(n)) *
 *            (Fib(n)-Fib(n-1)+1) / 2 a(1) = 1, a(2) = 1, a(3) = 3,
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    // OEIS offset is 1; for n < 1 the result is undefined (return 0 here)
    if (n == 0) {
        return 0;
    }

    // Compute Fibonacci(n-1) and Fibonacci(n)
    seq_t low  = fib(n - 1); // Fib(n-1)
    seq_t high = fib(n);     // Fib(n)

    // Count of integers in the inclusive range [low, high]
    seq_t count = high - low + 1;

    // Sum of an arithmetic progression: (first + last) * count / 2
    seq_t sum = (low + high) * count / 2;

    return sum;
}
