/*
 * A214789  a(n) is the smallest k>=2 such that n divides A000045(k-1)+8.
 * https://oeis.org/A214789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k ≥ 2 such that n divides F(k‑1)+8, where
 *            F(0)=0, F(1)=1. The routine computes this by iterating Fibonacci
 *            numbers modulo n until the congruence holds
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* shortcut for n == 1, where the answer is trivially 2 */
    if (n == 1)
        return (seq_t)2;

    idx_t mod = n;
    idx_t f_prev = 0;          /* F(0) */
    idx_t f_curr = 1;          /* F(1) */

    for (idx_t m = 1; ; ++m)   /* m = k‑1, start with F(1) */
    {
        /* Compute (F(m)+8) mod n without overflow */
        idx_t rem;
        if (mod <= 8)                /* safe to add and take modulus */
            rem = (f_curr + 8) % mod;
        else
            rem = f_curr >= mod - 8 ? f_curr - (mod - 8) : f_curr + 8;

        if (rem == 0)
            return (seq_t)(m + 1);   /* k = m+1 */

        /* Next Fibonacci number modulo n (avoid overflow) */
        idx_t r;
        /* f_prev < mod, f_curr < mod, so 0 <= f_prev + f_curr < 2*mod */
        if (f_prev >= mod - f_curr)
            r = f_prev - (mod - f_curr);
        else
            r = f_prev + f_curr;

        f_prev = f_curr;
        f_curr = r;
    }
}
