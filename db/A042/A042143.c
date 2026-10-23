/*
 * A042143  Denominators of continued fraction convergents to sqrt(596).
 * https://oeis.org/A042143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuants (denominators) of the convergents of sqrt(596). */
/* The sequence is produced by the standard recurrence for a quadratic
   irrational continued fraction. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: the 0‑th denominator is always 1. */
    if (n == 0)
        return (seq_t)1;

    /* Constants for sqrt(596). */
    const idx_t D = 596;          /* the radicand */
    /* Compute a0 = floor(sqrt(D)).  For D=596, a0 = 24. */
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        ++a0;

    /* Continued‑fraction variables.  These fit into 64‑bit types. */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_val = a0;   /* a_0 */

    /* Convergent recurrence: p_-2=0, p_-1=1; q_-2=1, q_-1=0. */
    seq_t p_nm2 = (seq_t)0;    /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;    /* p_{-1} */
    seq_t q_nm2 = (seq_t)1;    /* q_{-2} */
    seq_t q_nm1 = (seq_t)0;    /* q_{-1} */

    /* Iterate until we reach the requested index. */
    for (idx_t i = 0; i <= n; ++i) {
        /* New convergent */
        seq_t p = a_val * p_nm1 + p_nm2;
        seq_t q = a_val * q_nm1 + q_nm2;

        /* If this is the answer, return it. */
        if (i == n)
            return q;

        /* Prepare the next partial quotient a_{i+1}. */
        m = d * a_val - m;
        d = (D - m * m) / d;                /* exact division */
        a_val = (a0 + m) / d;               /* integer division */

        /* Shift the recurrence registers. */
        p_nm2 = p_nm1;
        p_nm1 = p;
        q_nm2 = q_nm1;
        q_nm1 = q;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
