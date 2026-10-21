/*
 * A214002  Number of compositions of n into ceiling(n/2) parts with 1 <= each
 *          part <=4.
 * https://oeis.org/A214002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{h=0..floor((n-1)/4)} (-1)^h * binomial(n-4h-1,
 *            ceil(n/2)-1) * binomial(ceil(n/2), h)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_int(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= i; /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* not used, offset starts at 1 */
    idx_t m = (n + 1) / 2;               /* ceil(n/2) */
    idx_t maxh = (n - 1) / 4;            /* floor((n-1)/4) */
    seq_t sum = 0;

    for (idx_t h = 0; h <= maxh; ++h) {
        idx_t top = n - 4 * h - 1;
        if (top < m - 1) break;          /* binomial would be zero */
        seq_t c1 = binom_int(top, m - 1);
        seq_t c2 = binom_int(m, h);
        seq_t term = c1 * c2;

        if ((h & 1) == 0)
            sum += term;
        else
            sum -= term;
    }
    return sum;
}
