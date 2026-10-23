/*
 * A041051  Denominators of continued fraction convergents to sqrt(31).
 * https://oeis.org/A041051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to √31.
   Recurrence: a(0)=1, a(1)=1; for n≥2,
   a(n)=c[n]*a(n-1)+a(n-2) where
   c[0]=5 and for n≥1 the sequence c repeats
   1,1,3,5,3,1,1,10. */
seq_t a(idx_t n)
{
    static const seq_t period[8] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5,
                                    (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)10 };
    seq_t q_prevprev = (seq_t)1; /* q_{‑2} */
    seq_t q_prev     = (seq_t)0; /* q_{‑1} */
    seq_t q          = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)5;                 /* a₀ */
        else
            a_i = period[(i - 1) % 8];      /* aₙ for n≥1 */

        q = a_i * q_prev + q_prevprev;     /* aₙ = aₙ·aₙ₋₁ + aₙ₋₂ */
        q_prevprev = q_prev;
        q_prev = q;
    }
    return q;
}
