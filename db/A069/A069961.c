/*
 * A069961  Define C(n) by the recursion C(0) = 4*i where i^2 = -1, C(n+1) =
 *          1/(1 + C(n)), then a(n) = 4*(-1)^n/Im(C(n)) where Im(z) denotes
 *          the imaginary part of z.
 * https://oeis.org/A069961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*F(n)^2 + F(n+1)^2 (F(k) – k‑th Fibonacci number)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A069961. */
seq_t a(idx_t n)
{
    /* Fibonacci iterators: a = F(i), b = F(i+1) initially F(0)=0, F(1)=1 */
    seq_t a_f = 0;          /* F(i)   */
    seq_t b_f = 1;          /* F(i+1) */

    /* Advance i from 0 to n, keeping a_f = F(i), b_f = F(i+1) */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t tmp = a_f + b_f;  /* F(i+2) = F(i) + F(i+1) */
        a_f = b_f;
        b_f = tmp;
    }

    /* Now a_f = F(n),   b_f = F(n+1) */
    seq_t term = (a_f * a_f) * 16;   /* 16 * F(n)^2 */
    term += b_f * b_f;               /* + F(n+1)^2 */

    return term;
}
