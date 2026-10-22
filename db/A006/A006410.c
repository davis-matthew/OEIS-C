/*
 * A006410  Number of nonseparable rooted toroidal maps with n + 5 edges and n
 *          + 1 vertices.
 * https://oeis.org/A006410
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*binomial(n+6,8) + 471*binomial(n+6,9) +
 *            2734*binomial(n+6,10) + 5388*binomial(n+6,11) +
 *            3264*binomial(n+6,12)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 106, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 6;
    seq_t c = 1;
    idx_t i;
    /* c = binomial(m, 8), built so that c*(m-8+i) = i*binomial(m-8+i, i) */
    for (i = 1; i <= 8; i++) {
        c = c * (m - 8 + i) / i;
    }
    seq_t r = 20 * c;
    c = c * (m - 8) / 9;   /* binomial(m, 9) */
    r += 471 * c;
    c = c * (m - 9) / 10;  /* binomial(m, 10) */
    r += 2734 * c;
    c = c * (m - 10) / 11; /* binomial(m, 11) */
    r += 5388 * c;
    c = c * (m - 11) / 12; /* binomial(m, 12) */
    r += 3264 * c;
    return r;
}
