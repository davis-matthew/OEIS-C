/*
 * A210524  a(n) = n - sum of even digits of n.
 * https://oeis.org/A210524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n - sum of even digits of n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t > 0)
    {
        int digit = t % 10;
        if ((digit & 1) == 0)
            sum += (seq_t)digit;
        t /= 10;
    }
    return (seq_t)n - sum;
}
