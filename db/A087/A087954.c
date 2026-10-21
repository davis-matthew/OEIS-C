/*
 * A087954  (2 + phi)/a(n) is the sum of successive remainders when computing
 *          the Euclidean algorithm for (1, A088166(n)/phi) with phi being the
 *          golden ratio, where n >= 2.
 * https://oeis.org/A087954
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Lucas(4*n) - 2 if n ≡ 0 (mod 3) * a(n) = Lucas(4*n) -
 *            Lucas(2*n) otherwise * where Lucas(k) = Fib(k-1) + Fib(k+1).
 *            Offset: a(2) is the first term
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Sequence is only defined for n >= 2; values for smaller n are unused. */
    /* Compute Lucas(4*n) as Fib(4*n-1) + Fib(4*n+1) */
    seq_t l4 = fib(4 * n - 1) + fib(4 * n + 1);

    if ((n % 3) == 0) {
        /* a(n) = Lucas(4*n) - 2 */
        return l4 - (seq_t)2;
    } else {
        /* a(n) = Lucas(4*n) - Lucas(2*n) */
        seq_t l2 = fib(2 * n - 1) + fib(2 * n + 1);
        return l4 - l2;
    }
}
