/*
 * A035755  Coordination sequence for C_18 lattice.
 * https://oeis.org/A035755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^18
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 23, u128 n <= 318,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(n,k) exactly in seq_t */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t pos = 2 * n ;                 /* 2n is the exponent of x */
    idx_t lim = (pos < 18) ? pos : 18; /* i runs up to min(18,2n) */
    seq_t result = 0;

    for (idx_t i = 0; i <= lim; ++i) {
        seq_t term1 = binom_calc(18, i);                    /* C(18,i) */
        seq_t term2 = binom_calc(17 + pos - i, pos - i);   /* C(17+2n-i,2n-i) */
        result += term1 * term2;
    }
    return result;
}
