/*
 * A041022  Numerators of continued fraction convergents to sqrt(15).
 * https://oeis.org/A041022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 3 a(1) = 4 For n >= 2: a(n) = coeff(n) * a(n-1) + a(n-2)
 *            where coeff(n) = 1 if n is odd, 6 if n is even. This follows
 *            from the recurrence for convergents of the continued fraction of
 *            sqrt(15) with coefficients [3; 1,6,1,6,...]
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of A041022. */
seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 0U) return (seq_t)3;
    if (n == 1U) return (seq_t)4;

    seq_t p_nm2 = (seq_t)0;   /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;   /* p_{-1} */
    seq_t p_k   = 0;

    /* iterate from k = 0 up to n, computing successive convergent numerators */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff;
        if (k == 0U) {
            coeff = (seq_t)3;
        } else {
            /* for k >= 1: coeff alternates 1,6,1,6,... */
            coeff = (k & 1U) ? (seq_t)1 : (seq_t)6;
        }
        p_k = coeff * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }
    return p_k;
}
