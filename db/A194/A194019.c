/*
 * A194019  E.g.f. = exp(-x*(x+4)/2)/(1-x)^3.
 * https://oeis.org/A194019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)*a(n-2) + (n-1)*(n-2)*a(n-3); initial
 *            values: a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* use a sliding window of the last three values */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)3;   /* a(n-1) */
    seq_t ai;                 /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t term1 = (seq_t)i           * a_nm1;          /* n*a(n-1)          */
        seq_t term2 = (seq_t)(i - 1)     * a_nm2;          /* (n-1)*a(n-2)      */
        seq_t term3 = (seq_t)(i - 1) * (seq_t)(i - 2) * a_nm3; /* (n-1)(n-2)*a(n-3)*/
        ai = term1 + term2 + term3;       /* a(n) = term1 + term2 + term3 */

        /* shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }

    return a_nm1;  /* a_nm1 holds a(n) after the loop */
}
