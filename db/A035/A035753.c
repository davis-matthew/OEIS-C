/*
 * A035753  Coordination sequence for C_16 lattice.
 * https://oeis.org/A035753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^16
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 29, u128 n <= 568,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pre-computed binomial coefficients for (16 choose i), i=0..16 */
static const seq_t _BINOM16[17] = {
    1ULL, 16ULL, 120ULL, 560ULL, 1820ULL, 4368ULL,
    8008ULL, 11440ULL, 12870ULL, 11440ULL, 8008ULL,
    4368ULL, 1820ULL, 560ULL, 120ULL, 16ULL, 1ULL
};

/* Compute binomial coefficient C(n,k) for arbitrary-precision seq_t.
 * The loop is bounded by min(k, n-k); for our usage this is always <= 15. */
static seq_t _binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Coordination sequence for the C_16 lattice */
seq_t a(idx_t n)
{
    /* 2n might overflow for astronomically large n; that case is not needed.
       For practical n (up to about 10000) the following works safely. */
    idx_t two_n = n * 2;

    seq_t sum = 0;
    idx_t max_i = (two_n < 16) ? two_n : 16;  /* i runs from 0 to min(16,2n) */

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t j = two_n - i;                     /* exponent for (1-x)^(-16) */
        seq_t bin1 = _BINOM16[i];
        seq_t bin2 = _binom_seq(15 + j, j);      /* C(15+j, j) = C(15+j, 15) when j>15 */
        sum += bin1 * bin2;
    }

    return sum;
}
