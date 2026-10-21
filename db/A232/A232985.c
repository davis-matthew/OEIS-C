/*
 * A232985  The Gauss factorial n_11!.
 * https://oeis.org/A232985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤j≤n, gcd(j,11)=1} j
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t j = 1; j <= n; ++j)
    {
        if (j % 11 != 0)
            res = res * j;
    }
    return res;
}
