/*
 * A024816  Antisigma(n): Sum of the numbers less than n that do not divide n.
 * https://oeis.org/A024816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n*(n+1)/2 - sigma(n)
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma_of(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t sum  = 1;
            while (m % p == 0) {
                m /= p;
                term *= (seq_t)p;
                sum  += term;
            }
            result *= sum;
        }
    }
    if (m > 1) {
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

seq_t a(idx_t n) {
    /* offset: a(1) = 0 */
    if (n < 1) return 0;
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t sigma = sigma_of(n);
    return tri - sigma;
}
