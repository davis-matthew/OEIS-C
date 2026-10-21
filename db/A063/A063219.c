/*
 * A063219  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          53 ).
 * https://oeis.org/A063219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A063219 – Dimension of the space of weight 2n cuspidal newforms for Γ₀(53)
    a(n) = a(n-1) + a(n-3) - a(n-4)  for n > 5
    a(1) =  4 , a(2) = 13 , a(3) = 21 , a(4) = 31 , a(5) = 39
    The function computes the n‑th term for any n ≥ 1. */

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Return the pre‑computed initial values when n ≤ 5. */
    if (n <= 5) {
        static const seq_t init[5] = { (seq_t)4, (seq_t)13, (seq_t)21, (seq_t)31, (seq_t)39 };
        return init[n-1];
    }

    /* Variables hold the last four computed terms:
       a5 = a(i-1), a4 = a(i-2), a3 = a(i-3), a2 = a(i-4). */
    seq_t a5 = (seq_t)39;   /* a(5) */
    seq_t a4 = (seq_t)31;   /* a(4) */
    seq_t a3 = (seq_t)21;   /* a(3) */
    seq_t a2 = (seq_t)13;   /* a(2) */

    /* Iterate from i = 6 up to n, applying the recurrence. */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t ai = a5 + a3 - a2;  /* a(i-1) + a(i-3) - a(i-4) */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }

    return a5;
}
