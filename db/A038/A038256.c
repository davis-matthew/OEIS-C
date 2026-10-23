/*
 * A038256  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*2^j.
 * https://oeis.org/A038256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 6^(i-j) * 2^j , * where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2. * The triangle
 *            is enumerated row by row (i from 0, j from 0..i)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the row index i by scanning cumulated triangular numbers. */
    idx_t i = 0;
    while (i * (i + 1) / 2 <= n)
        ++i;
    --i;                                   /* now i*(i+1)/2 <= n < (i+1)*(i+2)/2 */

    /* Column index j within that row. */
    idx_t j = n - i * (i + 1) / 2;

    /* Compute binomial(i,j) multiplicatively. */
    seq_t binom = 1;
    idx_t k = j;
    /* Use the formula: C(i,k) = Π_{t=1..k} (i - k + t) / t */
    for (idx_t t = 1; t <= k; ++t)
        binom = binom * ( (seq_t)(i - k + t) ) / (seq_t)t;

    /* Compute 6^(i-j) and 2^j by repeated multiplication. */
    seq_t pow6 = 1;
    for (idx_t p = 0; p < i - j; ++p)
        pow6 *= (seq_t)6;

    seq_t pow2 = 1;
    for (idx_t p = 0; p < j; ++p)
        pow2 *= (seq_t)2;

    /* Final result. */
    return binom * pow6 * pow2;
}
