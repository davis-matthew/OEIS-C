/*
 * A041870  Numerators of continued fraction convergents to sqrt(457).
 * https://oeis.org/A041870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numerators of continued‑fraction convergents to sqrt(457).
   Recurrence: p_n = a_n * p_{n-1} + p_{n-2}
   partial quotients: a_0 = 21, for n>=1  a_n repeats the period
   [2,1,1,1,5,2,13,1,3,1,4,1,1,4,1,3,1,13,2,5,1,1,1,2,42] (length 25). */

seq_t a(idx_t n)
{
    /* Periodic continued–fraction coefficients after the first term. */
    static const unsigned char period[25] = {
        2,1,1,1,5,2,13,1,3,1,4,1,1,4,1,3,1,13,2,5,1,1,1,2,42
    };
    /* p_{-2} = 0, p_{-1} = 1   (by convention for continued fractions) */
    seq_t p_nm2 = 0;       /* holds p_{i-2} */
    seq_t p_nm1 = 1;       /* holds p_{i-1} */
    seq_t p = 0;           /* current p_i */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)21;                /* a_0 */
        else
            a_i = (seq_t)period[(i - 1) % 25]; /* a_i for i >= 1 */

        /* p_i = a_i * p_{i-1} + p_{i-2} */
        p = a_i * p_nm1 + p_nm2;

        /* shift for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p;
    }
    return p;   /* p_n */
}
