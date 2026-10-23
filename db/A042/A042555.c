/*
 * A042555  Denominators of continued fraction convergents to sqrt(806).
 * https://oeis.org/A042555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of sqrt(806)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 
 * Computes a(n) for the OEIS sequence A042555:
 * a(n) = denominator of the n-th convergent to sqrt(806).
 * The recurrence a(n) = 12332790*a(n-14) - a(n-28) (for n>27)
 * is a consequence of the continued‑fraction expansion, but the
 * implementation below computes the values directly via that
 * expansion, which works for all n ≥ 0.
 */
seq_t a(idx_t n)
{
    /* 806 is not a perfect square, floor(sqrt(806)) = 28 */
    const idx_t D = 806;
    const idx_t a0 = 28;

    idx_t m = 0;            /* m_{k-1} in the standard CF algorithm */
    idx_t d = 1;            /* d_{k-1}                          */
    idx_t a = a0;           /* partial quotient a_k              */

    seq_t p_m2 = 0;   /* p_{k-2}  */
    seq_t p_m1 = 1;   /* p_{k-1}  */
    seq_t q_m2 = 1;   /* q_{k-2}  */
    seq_t q_m1 = 0;   /* q_{k-1}  */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Compute p_k and q_k for this k */
        seq_t p = (seq_t)a * p_m1 + p_m2;
        seq_t q = (seq_t)a * q_m1 + q_m2;

        if (i == n)
            return q;          /* denominator of the n-th convergent */

        /* Prepare values for the next step of the continued fraction */
        idx_t new_m = d * a - m;
        idx_t new_d = (D - new_m * new_m) / d;
        idx_t new_a = (a0 + new_m) / new_d;

        /* Shift the storage for the next iteration */
        p_m2 = p_m1;  p_m1 = p;
        q_m2 = q_m1;  q_m1 = q;

        m = new_m;
        d = new_d;
        a = new_a;
    }

    /* Control should never reach here; return 0 on safety. */
    return (seq_t)0;
}
