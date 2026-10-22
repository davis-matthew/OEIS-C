/*
 * A004976  a(n) = floor(n*phi^3), where phi=(1+sqrt(5))/2.
 * https://oeis.org/A004976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi^3) = floor(n*(2+sqrt(5))) = 2*n +
 *            floor(sqrt(5*n^2))
 * Range:     u8 n <= 7, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_isqrt(seq_t x) {
    seq_t r, y;
    if (x < 2) return x;
    r = x;
    y = (r + x / r) / 2;
    while (y < r) {
        r = y;
        y = (r + x / r) / 2;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return 2 * m + seq_isqrt((seq_t)5 * m * m);
}
