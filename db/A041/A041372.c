/*
 * A041372  Numerators of continued fraction convergents to sqrt(201).
 * https://oeis.org/A041372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of the continued fraction convergents to
 *            sqrt(201)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial quotients: a0 = 14; subsequent values repeat the period */
/* [5,1,1,1,2,1,8,1,2,1,1,1,5,28] */
seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(201) */
    static const int period[14] =
    {
        5, 1, 1, 1, 2, 1, 8, 1, 2, 1, 1, 1, 5, 28
    };

    /* Handle the first term separately */
    if (n == 0)
        return (seq_t)14;

    /* Compute successive convergents */
    seq_t prevprev = (seq_t)0;   /* p_{-2} */
    seq_t prev     = (seq_t)1;   /* p_{-1} */
    seq_t cur;
    idx_t i;

    for (i = 0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)14;                       /* a_0 */
        else
        {
            idx_t idx = (i - 1) % 14;               /* periodic index */
            a_i = (seq_t)period[(int)idx];           /* a_i for i >= 1 */
        }

        cur = a_i * prev + prevprev;   /* p_i = a_i * p_{i-1} + p_{i-2} */

        /* Shift for next iteration */
        prevprev = prev;
        prev     = cur;
    }

    return prev;   /* prev holds p_n after the loop */
}
