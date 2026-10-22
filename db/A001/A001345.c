/*
 * A001345  a(n) = Sum_{k = 0..3} (n+k)! C(3,k).
 * https://oeis.org/A001345
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k = 0..3} (n+k)! C(3,k) = n! * (1 + 3(n+1) +
 *            3(n+1)(n+2) + (n+1)(n+2)(n+3)) for n >= 0, a(-1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t f, m, poly;
    idx_t i;
    if (n < 0) {
        return (seq_t)7;
    }
    f = (seq_t)1;
    for (i = 2; i <= n; i++) {
        f *= (seq_t)i;
    }
    m = (seq_t)n + 1;
    poly = (seq_t)1 + (seq_t)3 * m + (seq_t)3 * m * (m + 1) + m * (m + 1) * (m + 2);
    return f * poly;
}
