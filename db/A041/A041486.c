/*
 * A041486  Numerators of continued fraction convergents to sqrt(260).
 * https://oeis.org/A041486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041486: Numerators of continued‑fraction convergents
 * to √260.
 *
 * Recurrence implementation:
 *   Let D = 260, a0 = ⌊√D⌋ = 16.
 *   Set m0 = 0, d0 = 1, a0 = a0.
 *   For k ≥ 0:
 *     pk   = ak * pk-1 + pk-2   (numerator),
 *     qk   = ak * qk-1 + qk-2   (denominator),
 *     mk+1 = dk * ak - mk,
 *     dk+1 = (D - mk+1²) / dk,
 *     ak+1 = (a0 + mk+1) / dk+1   (floor division).
 *
 *   Initial conditions:
 *     p-2 = 0, p-1 = 1;   q-2 = 1, q-1 = 0.
 *
 *   a(0) = p0 = 16, a(1) = 129, a(2) = 4144, …                     */
seq_t a(idx_t n)
{
    const int D   = 260;     /* non‑square integer */
    const int a0  = 16;     /* floor(sqrt(D)) */

    /* Current continued‑fraction state */
    int m = 0;
    int d = 1;
    int a = a0;

    /* Recurrence states for numerators and denominators */
    seq_t p_prev_prev = 0, /* p_{k-2} */
          p_prev      = 1, /* p_{k-1} */
          p = 0;           /* p_k */
    seq_t q_prev_prev = 1, /* q_{k-2} */
          q_prev      = 0, /* q_{k-1} */
          q = 0;           /* q_k */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Compute current numerator p_k */
        p = (seq_t)a * p_prev + p_prev_prev;
        q = (seq_t)a * q_prev + q_prev_prev;

        if (i == n)
            return p;

        /* Compute next partial quotient a_{k+1} */
        int m_next = d * a - m;
        int d_next = (D - m_next * m_next) / d;
        int a_next = (a0 + m_next) / d_next;

        /* Shift recurrence states for next iteration */
        p_prev_prev = p_prev;
        p_prev      = p;
        q_prev_prev = q_prev;
        q_prev      = q;

        /* Update continued‑fraction state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* Unreachable, but required for completeness */
    return 0;
}
