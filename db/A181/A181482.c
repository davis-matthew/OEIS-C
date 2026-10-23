/*
 * A181482  The sum of the first n integers, with every third integer taken
 *          negative.
 * https://oeis.org/A181482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - 3*floor(n/3)*(floor(n/3)+1)
 * Range:     u8 n <= 37, u16 n <= 625, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n / 3;                      /* floor(n/3) */
    seq_t sum1 = (seq_t)n * ((seq_t)n + (seq_t)1) / 2;   /* n*(n+1)/2 */
    seq_t sub = (seq_t)3 * ((seq_t)m * ((seq_t)m + (seq_t)1)); /* 3*m*(m+1) */
    return sum1 - sub;
}
