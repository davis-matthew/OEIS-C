/*
 * A038328  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*2^j.
 * https://oeis.org/A038328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*12^(i-j)*2^j where * i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2 * #i is the row
 *            index, #j is the column index within that row
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 170, u128 n <= 630,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row i such that (i+1)(i+2)/2 > n  */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }

    /* offset j within the chosen row */
    idx_t prev = i * (i + 1) / 2;
    idx_t j = n - prev;

    /* compute binomial(i, j) multiplicatively, using symmetry to reduce work */
    idx_t k = j;
    if (k > i - j) {
        k = i - j;            /* use the smaller of j and i-j */
    }

    seq_t bin = 1;
    for (idx_t t = 1; t <= k; ++t) {
        bin = bin * (seq_t)(i + 1 - t) / (seq_t)t;
    }

    /* multiply by 12^(i-j) */
    seq_t res = bin;
    for (idx_t e = 0; e < i - j; ++e) {
        res = res * (seq_t)12;
    }

    /* multiply by 2^j */
    for (idx_t e = 0; e < j; ++e) {
        res = res * (seq_t)2;
    }

    return res;
}
