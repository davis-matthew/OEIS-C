/*
 * A087287  a(n) = Lucas(9*n).
 * https://oeis.org/A087287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Lucas(9*n) Compute via Fibonacci fast‑doubling: L(k) =
 *            F(k-1)+F(k+1), with fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
 *            giving F(n) and F(n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n) {
    /* m = 9*n, use idx_t arithmetic to avoid overflow */
    idx_t m = n * 9;

    /* fib_pair returns F(m) and F(m+1) */
    seq_t fm, fm1;
    fib_pair(m, &fm, &fm1);

    /* F(m-1) = F(m+1) - F(m) */
    seq_t fm_minus1 = fm1 - fm;

    /* L(m) = F(m-1) + F(m+1) */
    return fm_minus1 + fm1;
}
