/*
 * A014166  Apply partial sum operator 4 times to Fibonacci numbers.
 * https://oeis.org/A014166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+8) - (n^3 +12*n^2 +59*n +126)/6
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t fn = fib(n + 8);
    seq_t nn = (seq_t)n;
    seq_t poly = nn * nn * nn + 12 * nn * nn + 59 * nn + 126;
    seq_t div  = poly / 6;
    return fn - div;
}
