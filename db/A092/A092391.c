/*
 * A092391  a(n) = n + wt(n), where wt(n) = A000120(n) = binary weight of n.
 * https://oeis.org/A092391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + A000120(n) – binary weight of n
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t popcount_idx(idx_t x)
{
    seq_t cnt = 0;
    while (x != 0) {
        cnt += (seq_t)(x & 1U);
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t w = popcount_idx(n);
    return (seq_t)n + w;
}
