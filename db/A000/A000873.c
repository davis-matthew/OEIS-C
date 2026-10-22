/*
 * A000873  Numbers beginning with letter 'e' in English.
 * https://oeis.org/A000873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   a(n) = n-th number whose English name begins with 'e', counted
 *            by blocks. The name begins with the name of the leading 3-digit
 *            group g (the value m / 1000^k for the largest k where it is >=
 *            1), so it starts with 'e' iff g is 8, 11, 18, 80..89 or
 *            800..899. Each level k gives the ranges [lo*1000^k,...
 * Range:     u8 n <= 13, u16 n <= 3113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned a873_lo[5] = {8, 11, 18, 80, 800};
static const unsigned a873_hi[5] = {8, 11, 18, 89, 899};

seq_t a(idx_t n) {
    idx_t r = n - 1;
    idx_t block = 1;
    seq_t bs = (seq_t)1;
    for (;;) {
        int i;
        for (i = 0; i < 5; i++) {
            idx_t w = (idx_t)(a873_hi[i] - a873_lo[i] + 1);
            if (r / block < w) {
                return (seq_t)a873_lo[i] * bs + (seq_t)r;
            }
            r -= w * block;
        }
        block *= 1000;
        bs *= 1000;
    }
}
