/*
 * A049602  a(n) = (Fibonacci(2*n)-(-1)^n*Fibonacci(n))/2.
 * https://oeis.org/A049602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(2*n) - (-1)^n*Fibonacci(n))/2 Recurrence:
 *            a(n)=2a(n-1)+3a(n-2)-4a(n-3)+a(n-4) (not used here)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute Fibonacci(n) and Fibonacci(2*n) using the provided fast routine */
    seq_t fn = fib(n);
    seq_t f2n = fib(2 * n);

    seq_t numer;
    if ((n & 1U) == 0) {          /* even n: (-1)^n = 1  */
        numer = f2n - fn;
    } else {                      /* odd n:  (-1)^n = -1 */
        numer = f2n + fn;
    }
    return numer / 2;
}
