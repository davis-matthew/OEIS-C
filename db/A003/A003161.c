/*
 * A003161  A binomial coefficient sum.
 * https://oeis.org/A003161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (binomial(n,k) - binomial(n,k-1))^3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;   /* k = 0 term: (1 - 0)^3 */
    seq_t prev = 1;     /* binomial(n, k-1) */
    idx_t half_n = n / 2;
    idx_t k;

    for (k = 1; k <= half_n; k++) {
        seq_t cur = prev * (seq_t)(n - k + 1) / (seq_t)k;
        seq_t diff = cur - prev;
        result += diff * diff * diff;
        prev = cur;
    }

    return result;
}
