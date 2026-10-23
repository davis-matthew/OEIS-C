/*
 * A041116  Numerators of continued fraction convergents to sqrt(67).
 * https://oeis.org/A041116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of continued fraction convergents to sqrt(67)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p_{-2}=0, p_{-1}=1, p_n = a_n * p_{n-1} + p_{n-2}   */
/* with partial quotients a_0 = 8, a_1.. repeating pattern [5,2,1,1,7,1,1,2,5,16] */
seq_t a(idx_t n)
{
    /* periodic part of the continued fraction after the first term */
    static const seq_t period[10] = {5, 2, 1, 1, 7, 1, 1, 2, 5, 16};

    if (n == 0)
        return 8;            /* first numerator */

    seq_t p_nm2 = 0;          /* p_{-2} */
    seq_t p_nm1 = 1;          /* p_{-1} */
    seq_t p_n;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k = (k == 0) ? (seq_t)8 : period[(k - 1) % 10];
        p_n = a_k * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_nm1;             /* p_n */
}
