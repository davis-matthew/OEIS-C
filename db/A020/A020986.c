/*
 * A020986  a(n) = n-th partial sum of Golay-Rudin-Shapiro sequence A020985.
 * https://oeis.org/A020986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = sum_{k=0}^{n} (-1)^{popcount(k & (k >> 1))}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int _parity_idx(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t x = k & (k >> 1);
        int parity = _parity_idx(x);
        if (parity == 0)
            sum += (seq_t)1;
        else
            sum -= (seq_t)1;
    }
    return sum;
}
