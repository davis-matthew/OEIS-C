/*
 * A226141  Sum of the squared parts of the partitions of n into exactly two
 *          parts.
 * https://oeis.org/A226141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (i^2 + (n-i)^2)
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 1440, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses closed‑form sums of squares to compute the result in O(1). */
seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;
    /* Convert to seq_t for arithmetic */
    seq_t K  = (seq_t)k;
    seq_t nX = (seq_t)n;

    /* Sum_{i=1..k} i^2 = k(k+1)(2k+1)/6 */
    seq_t sum_i_sq = K * (K + 1) * (2 * K + 1) / 6;

    /* Sum of squares from 1 to n-1 */
    seq_t m1 = (seq_t)(n - 1);
    seq_t sum_m1 = m1 * (m1 + 1) * (2 * m1 + 1) / 6;

    /* Sum of squares from 1 to n-k-1 */
    idx_t m2_idx = n - k - 1;
    seq_t m2 = (seq_t)m2_idx;
    seq_t sum_m2 = m2 * (m2 + 1) * (2 * m2 + 1) / 6;

    /* a(n) = sum_i_sq + (sum_m1 - sum_m2) */
    return sum_i_sq + (sum_m1 - sum_m2);
}
