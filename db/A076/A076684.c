/*
 * A076684  Odd terms in A027941.
 * https://oeis.org/A076684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(6n+3) - 1, where F(k) denotes the k‑th Fibonacci number
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* compute k = 6*n + 3 */
    idx_t k = n * 6 + 3;

    seq_t fk, fk1;          /* fk = F(k), fk1 = F(k+1) */
    fib_pair(k, &fk, &fk1); /* provided by the utils library */

    return fk - (seq_t)1;
}
