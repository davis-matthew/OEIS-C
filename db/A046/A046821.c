/*
 * A046821  Number of 1's in binary expansion of 5n+1.
 * https://oeis.org/A046821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = popcount(5n + 1)
 * Range:     u8 n <= 50, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 5 + 1;
    seq_t cnt = 0;
    while (val != 0)
    {
        cnt++;
        val &= val - 1;
    }
    return cnt;
}
