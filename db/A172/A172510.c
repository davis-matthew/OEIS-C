/*
 * A172510  a(n) = binomial(n + 4, 4) * 8^n.
 * https://oeis.org/A172510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 4, 4) * 8^n (OEIS A172510)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute binomial(n+4, 4) = (n+4)(n+3)(n+2)(n+1) / 24 via successive
       multiplications/divisions by small integers */
    seq_t sn = (seq_t)n;
    seq_t binom = 1;
    for (idx_t k = 1; k <= 4; ++k) {
        binom = binom * (sn + k);
        binom = binom / k;
    }

    /* compute 8^n iteratively */
    seq_t pow8 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow8 = pow8 * 8;
    }

    return binom * pow8;
}
