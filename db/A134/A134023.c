/*
 * A134023  Number of zeros in balanced ternary representation of n.
 * https://oeis.org/A134023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zeros in balanced ternary representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t cnt = 0;
    while (n)
    {
        idx_t r = n % 3;
        n /= 3;
        if (r == 0)
            ++cnt;      /* zero digit */
        else if (r == 2)
            ++n;        /* carry for the -1 digit */
    }
    return cnt;
}
