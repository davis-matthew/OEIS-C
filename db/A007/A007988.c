/*
 * A007988  Expansion of (x^6-x^5-x^4+2x^2)/((1-x^3)(1-x^2)^2(1-x)).
 * https://oeis.org/A007988
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(27*(-1)^n+41+16*n+2*n^2)/144)
 * Range:     u8 n <= 7, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t m = N + 1;
    seq_t P = 2 * N * N + 16 * N + ((n & 1) ? 14 : 68);
    /* floor(m*P/144) without forming the large product m*P */
    seq_t q1 = m / 144, r1 = m % 144;
    seq_t q2 = P / 144, r2 = P % 144;
    return q1 * P + r1 * q2 + (r1 * r2) / 144;
}
