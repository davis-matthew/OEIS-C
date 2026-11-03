/*
 * A004471  Nim product 6 * n.
 * https://oeis.org/A004471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(6,n), the nim product 6 (x) n, computed by
 *            recursive splitting on Fermat 2-powers D = 2^(2^k), using D (x)
 *            D = D ^ (D/2)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 917 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t nim_mul_w(seq_t x, seq_t y, int L) {
	if (L == 1) return x & y;
	int h = L / 2;
	seq_t mask = ((seq_t)1 << h) - 1;
	seq_t x1 = x >> h, x0 = x & mask;
	seq_t y1 = y >> h, y0 = y & mask;
	seq_t c = nim_mul_w(x0, y0, h);
	seq_t e = nim_mul_w(x1, y1, h);
	seq_t d = nim_mul_w(x0 ^ x1, y0 ^ y1, h);
	seq_t high = d ^ c;
	seq_t low = c ^ nim_mul_w(e, (seq_t)1 << (h - 1), h);
	return (high << h) | low;
}

seq_t a(idx_t n) {
	int L = 4;
	while (L < 64 && (n >> L) != 0) L *= 2;
	return nim_mul_w((seq_t)6, (seq_t)n, L);
}
