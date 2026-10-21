/*
 * A208176  a(n) = F(n+1)^2, if n>=0 is even (F=A000045) and a(n) =
 *          (L(2n+2)+8)/5, if n is odd (L=A000204).
 * https://oeis.org/A208176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n)^2 + F(n)*F(n+1) + 1, where F is the Fibonacci
 *            sequence (F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the supplied fast‑doubling helper fib_pair to obtain F(n) and F(n+1). */
#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn = F(n), fn1 = F(n+1) */
    return fn * fn + fn * fn1 + 1;    /* a(n) = F(n)^2 + F(n)*F(n+1) + 1 */
}
