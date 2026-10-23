/*
 * A037870  a(n) = (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*2^i} is base 2
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ|d(i)-e(i)| = number of zeros among the lowest
 *            popcount(n) bits of n
 * Range:     u8 n <= 8102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t popcount_idx_t(idx_t x)
{
    idx_t cnt = 0;
    while (x) {
        x &= x - 1;   /* clear lowest set bit */
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* sequence is defined only for n ≥ 1, but we handle n == 0 safely */
    if (n == 0) return (seq_t)0;

    /* number of 1‑bits in n */
    idx_t k = popcount_idx_t(n);

    /* build a mask with the lowest k bits set */
    seq_t mask = (seq_t)0;
    for (idx_t i = 0; i < k; ++i) {
        mask |= ((seq_t)1 << i);
    }

    /* low_bits contains the k lowest bits of n */
    seq_t low_bits = ((seq_t)n) & mask;

    /* count 1‑bits among those low bits */
    idx_t onesLow = popcount_idx_t((idx_t)low_bits);

    /* zeros among the lowest k bits = k – onesLow */
    return (seq_t)(k - onesLow);
}
