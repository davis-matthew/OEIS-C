/*
 * A002417  4-dimensional figurate numbers: a(n) = n*binomial(n+2, 3).
 * https://oeis.org/A002417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2*(n+1)*(n+2)/6
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 399, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n_plus_1 = sn + 1;
    seq_t n_plus_2 = sn + 2;
    
    /* Compute n*(n+1)*(n+2)/6 (binomial(n+2, 3)), then multiply by n */
    seq_t half_prod = (sn * n_plus_1) / 2;
    seq_t binom = (half_prod * n_plus_2) / 3;
    
    return sn * binom;
}
