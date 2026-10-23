/*
 * A041602  Numerators of continued fraction convergents to sqrt(319).
 * https://oeis.org/A041602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=17; for n>=1, a(n)=c[n]*a(n-1)+a(n-2), where c[0]=17 and
 *            the periodic continuation c[1..] = 1,6,5,1,4,3,1,3,4,1,5,6,1,34
 *            (length 14)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(319). */
    static const idx_t period[14] = {
        1, 6, 5, 1, 4, 3, 1, 3, 4, 1, 5, 6, 1, 34
    };

    /* Convergents recurrence: p_{-2}=0, p_{-1}=1 */
    seq_t p_nm2 = (seq_t)0,        /* p_{-2} */
          p_nm1 = (seq_t)1,        /* p_{-1} */
          p_n   = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)17;
        else
            coeff = (seq_t)period[(i - 1) % 14];

        p_n = coeff * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_n;
}
