/*
 * A079004  Least x>=3 such that F(x)==1 (mod 3^n) where F(x) denotes the x-th
 *          Fibonacci number (A000045).
 * https://oeis.org/A079004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^(n-2) - 2 for n ≥ 3 (with a(1) = 7, a(2) = 10)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses pow_seq_t from the utilities to handle arbitrary‑precision
   powers. */
#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)2)                   /* special small indices */
        return (n == (idx_t)1) ? (seq_t)7 : (seq_t)10;

    /* n ≥ 3: compute 4 * 3^(n-2) - 2 efficiently */
    seq_t p   = pow_seq_t((seq_t)3, n - (idx_t)2);
    seq_t res = p * (seq_t)4;
    res = res - (seq_t)2;
    return res;
}
