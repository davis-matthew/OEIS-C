/*
 * A041648  Numerators of continued fraction convergents to sqrt(343).
 * https://oeis.org/A041648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(343)
   Recurrence: p_{-1} = 1,  p_0 = 18;
                for n ≥ 1:  p_n = a_n * p_{n-1} + p_{n-2}
   where a_n are the partial quotients of sqrt(343) after the initial 18:
   period 16: 1, 1, 11, 1, 5, 3, 1, 17,
               1, 3, 5, 1, 11, 1, 1, 36. */

seq_t a(idx_t n)
{
    /* The periodic part of the continued‑fraction expansion of sqrt(343). */
    static const seq_t periodic[16] = {
        1, 1, 11, 1, 5, 3, 1, 17,
        1, 3, 5, 1, 11, 1, 1, 36
    };

    if (n == 0)
        return 18;

    seq_t p_minus2 = 1;   /* p_{-1} */
    seq_t p_minus1 = 18;  /* p_0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t a_i = periodic[(i - 1) % 16];
        seq_t p   = a_i * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;
    }

    return p_minus1;
}
