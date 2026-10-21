/*
 * A217873  a(n) = 4*n*(n^2 + 2)/3.
 * https://oeis.org/A217873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n^2 + 2)/3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;        /* n^2 */
    seq_t t  = N2 + (seq_t)2;/* n^2 + 2 */
    seq_t prod = N * t;      /* n*(n^2 + 2) */
    prod *= (seq_t)4;        /* 4*n*(n^2 + 2) */
    return prod / (seq_t)3;  /* division by 3 is exact */
}
