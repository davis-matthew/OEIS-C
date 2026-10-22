/*
 * A001786  Expansion of 1/((1+x)*(1-x)^11).
 * https://oeis.org/A001786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(k+10,10), i.e. a(n) =
 *            binomial(n+10,10) - a(n-1), a(0) = 1. binomial(k+10,10) is built
 *            by 11 chained partial sums (additions only), so arithmetic is
 *            exact modulo 2^w and exact for big integers
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 38, u64 n <= 403, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s[11];
    seq_t res = 0;
    int j;
    idx_t k;
    for (j = 0; j < 11; j++) s[j] = 0;
    for (k = 0; k <= n; k++) {
        /* s[j] after step k equals binomial(k+j, j) */
        s[0] = 1;
        for (j = 1; j < 11; j++) s[j] = s[j] + s[j - 1];
        res = s[10] - res;
    }
    return res;
}
