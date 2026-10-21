/*
 * A071325  Number of squares > 1 dividing n.
 * https://oeis.org/A071325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer squares > 1 that divide n
 *            (equivalently, count of k ≥ 2 such that k² | n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 2; k * k <= n; ++k)
    {
        if (n % (k * k) == 0)
            ++cnt;
    }
    return cnt;
}
