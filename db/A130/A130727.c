/*
 * A130727  List of triples 2n+1, 2n+3, 2n+2.
 * https://oeis.org/A130727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 2k + 1, a(3k + 1) = 2k + 3, a(3k + 2) = 2k + 2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    idx_t r = n % 3;
    seq_t base = (seq_t)k * 2 + 1;
    switch (r) {
        case 0: return base;       /* 2k + 1 */
        case 1: return base + 2;   /* 2k + 3 */
        case 2: return base + 1;   /* 2k + 2 */
    }
    return base; /* unreachable, but satisfies the compiler */
}
