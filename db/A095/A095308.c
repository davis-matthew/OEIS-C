/*
 * A095308  Number of walks of length n between two nodes at distance 3 in the
 *          cycle graph C_7.
 * https://oeis.org/A095308
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-4) Initial values:
 *            a(3)=1, a(4)=1, a(5)=5, a(6)=6 Recurrence valid for all n ≥ 7
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the behaviour is unspecified; return 0. */
    if (n < 3) return 0;

    if (n == 3) return 1;
    if (n == 4) return 1;
    if (n == 5) return 5;
    if (n == 6) return 6;

    /* Let a1 = a(i-4), a2 = a(i-3), a3 = a(i-2), a4 = a(i-1)   */
    seq_t a1 = 1; /* a(3) */
    seq_t a2 = 1; /* a(4) */
    seq_t a3 = 5; /* a(5) */
    seq_t a4 = 6; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t term1 = a4 + mul_small(a3, 4);                 /* a_{i-1} + 4*a_{i-2} */
        seq_t term2 = mul_small(a2, 3) + mul_small(a1, 2);   /* 3*a_{i-3} + 2*a_{i-4} */
        seq_t next  = term1 - term2;                         /* a_i */

        /* Shift values for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4; /* a(n) */
}
