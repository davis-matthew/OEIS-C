/*
 * A211265  Number of integer pairs (x,y) such that 0<x<y<=n and x*y<=n+1.
 * https://oeis.org/A211265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{y=2}^n min(y-1, floor((n+1)/y))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    if (n <= 1) return ans;
    seq_t n_plus_one = (seq_t)n + 1;
    for (idx_t y = 2; y <= n; ++y) {
        seq_t prod_limit = n_plus_one / (seq_t)y;
        seq_t order_limit = (seq_t)y - 1;
        seq_t limit = prod_limit < order_limit ? prod_limit : order_limit;
        if (limit > 0) ans += limit;
    }
    return ans;
}
