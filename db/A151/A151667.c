/*
 * A151667  Number of partitions of n into distinct powers of 5.
 * https://oeis.org/A151667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if every digit of n in base 5 is 0 or 1, otherwise 0
 * Range:     u8 n <= 9630, u16 n <= 9630, u32 n <= 9630, u64 n <= 9630,
 *            u128 n <= 9630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    while (x > 0)
    {
        if ((x % 5) > 1)
            return (seq_t)0;
        x /= 5;
    }
    return (seq_t)1;
}
