/*
 * A137773  Triangular sequence: The Fibonacci sequence on the diagonal, 1's
 *          at all other places.
 * https://oeis.org/A137773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if n is not a triangular number; a(n) = Fib(k) where
 *            k(k+1)/2 = n. Fib(1)=0, Fib(2)=1, Fib(3)=1, Fib(4)=2, …
 * Range:     u8 n <= 7, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t fibonacci(idx_t k)
{
    /* Return Fibonacci with Fib(1)=0, Fib(2)=1. */
    if (k == 1u) return (seq_t)0;
    if (k == 2u) return (seq_t)1;
    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = 3u; i <= k; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(8*n + 1)). */
    seq_t s = (seq_t)8 * (seq_t)n + (seq_t)1;          /* 8n+1 */
    seq_t sqrt_val = floor_sqrt(s);                   /* floor(sqrt(8n+1)) */
    idx_t root = (idx_t)sqrt_val;                     /* root fits in idx_t */

    /* Determine k such that T_k = k(k+1)/2 <= n < T_{k+1}. */
    idx_t k = (root - (idx_t)1) / (idx_t)2;

    /* Check if n is exactly the triangular number T_k. */
    seq_t tri = (seq_t)k * (k + (idx_t)1) / (idx_t)2;
    if (tri == (seq_t)n) {
        /* Return Fibonacci(k). */
        return fibonacci(k);
    } else {
        /* Off-diagonal entries are 1. */
        return (seq_t)1;
    }
}
