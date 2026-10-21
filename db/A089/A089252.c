/*
 * A089252  a(n) = ((2*n-1)!!/sqrt(3))*(integral_{x=0..sqrt(3/4)}
 *          1/(1-x^2)^(n+1/2) dx).
 * https://oeis.org/A089252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(5*n-7)*a(n-1) - 8*(2*n-3)*(n-2)*a(n-2), with a(1)=1,
 *            a(2)=6 (recurrence from OEIS A089252)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; undefined for n=0 – harmless */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;

    /* iterate from 3 up to n, storing the last two values only */
    seq_t an_2 = (seq_t)1; /* a(n-2) */
    seq_t an_1 = (seq_t)6; /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        /* term1 = 2*(5*i-7)*a(i-1) */
        seq_t term1 = (seq_t)2 * (seq_t)(5 * i - 7);
        term1 *= an_1;

        /* term2 = 8*(2*i-3)*(i-2)*a(i-2) */
        seq_t term2 = (seq_t)8 * (seq_t)(2 * i - 3) * (seq_t)(i - 2);
        term2 *= an_2;

        an = term1 - term2;

        an_2 = an_1;
        an_1 = an;
    }

    return an_1;
}
