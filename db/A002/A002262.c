/*
 * A002262  Triangle read by rows: T(n,k) = k, 0 <= k <= n, in which row n
 *          lists the first n+1 nonnegative integers.
 * https://oeis.org/A002262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = n - r*(r+1)/2, where r = (isqrt(1+8*n) - 1)/2
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t isqrt(uint64_t x) {
    if (x < 2) return x;
    uint64_t guess = x;
    while (1) {
        uint64_t next = (guess + x / guess) / 2;
        if (next >= guess) return guess;
        guess = next;
    }
}

seq_t a(idx_t n) {
    uint64_t n64 = n;
    uint64_t s = isqrt(1 + 8 * n64);
    uint64_t r = (s - 1) / 2;
    uint64_t row_start = r * (r + 1) / 2;
    uint64_t c = n64 - row_start;
    return (seq_t)c;
}
