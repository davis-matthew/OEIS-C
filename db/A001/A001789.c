/*
 * A001789  a(n) = binomial(n,3)*2^(n-3).
 * https://oeis.org/A001789
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n,3)*2^(n-3)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b = (seq_t)n;
    b = b * (seq_t)(n - 1) / 2;
    b = b * (seq_t)(n - 2) / 3;
    seq_t r = b;
    idx_t i;
    for (i = 3; i < n; i++)
        r += r;
    return r;
}
