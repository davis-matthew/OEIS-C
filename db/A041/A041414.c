/*
 * A041414  Numerators of continued fraction convergents to sqrt(222).
 * https://oeis.org/A041414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041414: numerators of continued‑fraction convergents to √222.
   Recurrence:
     p_{-2} = 0,   p_{-1} = 1
     p_n   = a_n * p_{n-1} + p_{n-2}   for n ≥ 0
   where a_0 = 14 and afterwards a_n repeats the 4‑term cycle
     1, 8, 1, 28   (so a_1 = 1, a_2 = 8, a_3 = 1, a_4 = 28, ...).
   The function returns p_n. */
seq_t a(idx_t n)
{
    seq_t p_prevprev = 0; /* p_{-2} */
    seq_t p_prev     = 1; /* p_{-1} */
    seq_t p = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coef;
        if (i == 0)
            coef = 14;      /* a_0 */
        else
        {
            idx_t j = (i - 1) % 4;   /* periodic part */
            if (j == 0)
                coef = 1;
            else if (j == 1)
                coef = 8;
            else if (j == 2)
                coef = 1;
            else
                coef = 28;
        }

        p = coef * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p;
    }

    return p;
}
