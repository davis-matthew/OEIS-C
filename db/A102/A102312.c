/*
 * A102312  a(n) = Fibonacci(5*n).
 * https://oeis.org/A102312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(5*n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"

seq_t a(idx_t n) {
    /* compute Fibonacci at index 5*n using fast‐doubling */
    idx_t m = 5ULL * n;          /* 5*n fits in idx_t for the intended range */
    seq_t f, f_next;
    fib_fast(m, &f, &f_next);    /* f contains F(m) */
    return f;
}
