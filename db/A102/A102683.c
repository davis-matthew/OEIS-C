/*
 * A102683  Number of digits 9 in decimal representation of n.
 * https://oeis.org/A102683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of digits 9 in decimal representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t count = 0;
    while (m > 0)
    {
        if (m % 10 == 9)
            ++count;
        m /= 10;
    }
    return count;
}
