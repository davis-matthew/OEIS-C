/*
 * A214659  a(n) = n*(7*n^2 - 3*n - 1)/3.
 * https://oeis.org/A214659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n^2 - 3*n - 1)/3
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 936, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert n to seq_t for precise arithmetic */
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;                       /* n^2 */
    seq_t seven_n_sq = (seq_t)7 * n_sq;         /* 7*n^2 */
    seq_t three_n = (seq_t)3 * sn;              /* 3*n */
    /* numerator = n * (7*n^2 - 3*n - 1) */
    seq_t numerator = sn * (seven_n_sq - three_n - (seq_t)1);
    /* divide by 3 (exact division) */
    return numerator / (seq_t)3;
}
