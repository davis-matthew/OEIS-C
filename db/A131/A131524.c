/*
 * A131524  Number of possible palindromic rows (or columns) in an n X n
 *          crossword puzzle.
 * https://oeis.org/A131524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(floor((n+3)/2)) - 1 (equivalently a(2n) =
 *            Fibonacci(n+1)-1, a(2n+1) = Fibonacci(n+2)-1)
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 92, u64 n <= 184,
 *            u128 n <= 370, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* n is >= 1 */
    idx_t m = (n + 3) >> 1;   /* floor((n+3)/2) */
    seq_t f = fib(m);
    return f - 1;
}
