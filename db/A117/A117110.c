/*
 * A117110  Expansion of x*(1+5*x)/((1+2*x+1)*(1-4*x)).
 * https://oeis.org/A117110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) (n≥3) Initial values: a(0)=0, a(1)=1,
 *            a(2)=7 Computes the n‑th term of OEIS sequence A117110
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly.  */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    /* Use recurrence iteratively.  */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) : starts with a(1) */
    seq_t a_nm1 = (seq_t)7;   /* a(n-1) : starts with a(2) */
    seq_t an;                 /* a(n)   */

    /* Iterate from 3 up to n inclusive.  */
    for (idx_t i = 3; i <= n; ++i) {
        an = a_nm1 * (seq_t)2 + a_nm2 * (seq_t)8;
        /* Shift: a_nm2 becomes a_nm1, a_nm1 becomes an.  */
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
