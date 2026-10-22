/*
 * A004962  a(n) = ceiling(n*phi^7), where phi is the golden ratio, A001622.
 * https://oeis.org/A004962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^7) = ceiling((29*n + sqrt(845*n^2))/2) for
 *            n >= 1, a(0) = 0; since sqrt(845*n^2) is irrational for n >= 1,
 *            a(n) = floor((29*n + isqrt(845*n^2))/2) + 1
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t m, seq_t guess)
{
    /* guess must be >= isqrt(m) and > 0 */
    seq_t x = guess;
    seq_t y = (x + m / x) / 2;
    while (y < x) {
        x = y;
        y = (x + m / x) / 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t m = N * N * 845;
    seq_t s = isqrt_seq(m, N * 30);
    return (N * 29 + s) / 2 + 1;
}
