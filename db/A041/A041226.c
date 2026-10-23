/*
 * A041226  Numerators of continued fraction convergents to sqrt(125).
 * https://oeis.org/A041226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=11, a(1)=56, a(n)=c[(n-1)%5]*a(n-1)+a(n-2) where
 *            c={5,1,1,5,22} is the periodic sequence of partial quotients of
 *            the continued fraction of sqrt(125)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)11;

    /* periodic coefficients a1..a5 for sqrt(125) */
    static const seq_t coeff[5] = {(seq_t)5, (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)22};

    seq_t p_prev2 = (seq_t)1;   /* p_{-1} */
    seq_t p_prev1 = (seq_t)11;  /* p_{0} */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t an = coeff[(i - 1) % 5];
        seq_t next = an * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = next;
    }
    return p_prev1;
}
