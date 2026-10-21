/*
 * A085563  Sum of the prime digits of n.
 * https://oeis.org/A085563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of prime digits of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;
    while (temp > 0)
    {
        uint8_t d = temp % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7)
            sum += d;
        temp /= 10;
    }
    return sum;
}
