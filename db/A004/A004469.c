/*
 * A004469  Nim product 4 * n.
 * https://oeis.org/A004469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(4,n), nim product of 4 and n, computed with the
 *            Fermat-2-power split: for x,y < 2^(2^k), D = 2^(2^(k-1)), x =
 *            x1*D + x0, y = y1*D + y0 (XOR-combined), x*y = (x1*y1 + x1*y0 +
 *            x0*y1)*D + x0*y0 + (x1*y1)*(D/2), where + is XOR and * is nim
 *            product
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t nim_mul(seq_t x, seq_t y, unsigned bits) {
    if (x == 0 || y == 0) return 0;
    if (bits == 1) return x & y;
    if (x == 1) return y;
    if (y == 1) return x;
    unsigned half = bits / 2;
    seq_t D = (seq_t)1 << half;
    seq_t mask = D - 1;
    seq_t x1 = x >> half, x0 = x & mask;
    seq_t y1 = y >> half, y0 = y & mask;
    seq_t p = nim_mul(x1, y1, half);
    seq_t hi = p ^ nim_mul(x1, y0, half) ^ nim_mul(x0, y1, half);
    seq_t lo = nim_mul(x0, y0, half) ^ nim_mul(p, D >> 1, half);
    return (hi << half) ^ lo;
}

static seq_t nim_product(seq_t x, seq_t y) {
    seq_t m = x > y ? x : y;
    unsigned len = 0;
    seq_t t = m;
    while (t != 0) { t >>= 1; len++; }
    unsigned bits = 1;
    while (bits < len) bits *= 2;
    return nim_mul(x, y, bits);
}

seq_t a(idx_t n) {
    return nim_product((seq_t)4, (seq_t)n);
}
