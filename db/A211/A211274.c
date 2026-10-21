/*
 * A211274  Number of integer pairs (x,y) such that 0 < x <= y <= n and x*y <=
 *          3n.
 * https://oeis.org/A211274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{y=1}^{n} min(y, ⌊3n/y⌋)
 * Range:     u8 n <= 47, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t total = 0;
    seq_t three_n = (seq_t)n * 3;
    for (idx_t y = 1; y <= n; ++y)
    {
        seq_t limit = three_n / (seq_t)y;
        seq_t add = limit;
        if (add > (seq_t)y) add = (seq_t)y;
        total += add;
    }
    return total;
}
