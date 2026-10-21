/*
 * A071649  Sum of odd decimal digits of n.
 * https://oeis.org/A071649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of odd decimal digits of n
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n != 0)
    {
        unsigned d = n % 10;
        if (d & 1)
            sum += d;
        n /= 10;
    }
    return sum;
}
