/*
 * A042930  Numerators of continued fraction convergents to sqrt(997).
 * https://oeis.org/A042930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042930: Numerators of continued fraction convergents to sqrt(997).
   Recurrence: let a0 = ⌊√997⌋ = 31.
   For k ≥ 1, let a_k be the k‑th partial quotient of √997.
   Define p_{-2} = 0, p_{-1} = 1.
   For n ≥ 0, p_n = a_n * p_{n-1} + p_{n-2}.
   This function returns p_n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for √997 continued fraction */
    const uint64_t N = 997;       /* the radicand */
    const uint64_t A0 = 31;       /* floor(√997) */

    /* Continued fraction state */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = A0;              /* current partial quotient */

    /* Convergent recurrence state */
    seq_t p_im2 = (seq_t)0;       /* p_{-2} */
    seq_t p_im1 = (seq_t)1;       /* p_{-1} */
    seq_t p_i;                    /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        /* Compute current numerator */
        p_i = ((seq_t)a) * p_im1 + p_im2;

        /* Return if we reached the desired index */
        if (i == n) {
            return p_i;
        }

        /* Prepare next partial quotient */
        m = d * a - m;
        d = (N - m * m) / d;
        a = (A0 + m) / d;

        /* Shift recurrence state for the next iteration */
        p_im2 = p_im1;
        p_im1 = p_i;
    }

    /* Should never reach here */
    return (seq_t)0;
}
