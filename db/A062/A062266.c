/*
 * A062266  Row sums of unsigned triangle A062140 (generalized a=4 Laguerre).
 * https://oeis.org/A062266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+2)*a(n-1) - (n-1)*(n+3)*a(n-2) with initial values
 *            a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)6; /* a(1) */
    seq_t ai = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 2 * (i+2) * a(i-1) */
        seq_t term1 = ((seq_t)i + 2) * a1;
        term1 = term1 * 2;

        /* term2 = (i-1) * (i+3) * a(i-2) */
        seq_t term2 = ((seq_t)i - 1) * a0;
        term2 = term2 * ((seq_t)i + 3);

        ai = term1 - term2;
        a0 = a1;
        a1 = ai;
    }

    return ai;
}
