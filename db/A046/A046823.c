/*
 * A046823  Number of 1's in binary expansion of 5n+3.
 * https://oeis.org/A046823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = popcount(5*n + 3)
 * Range:     u8 n <= 50, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 5 + 3;
    seq_t count = 0;
    while (x)
    {
        x &= x - 1;
        count++;
    }
    return count;
}
