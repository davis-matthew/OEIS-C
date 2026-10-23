/*
 * A199208  a(n) = 9*4^n+1.
 * https://oeis.org/A199208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*4^n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 4;
    while (n > 0)
    {
        if (n & 1)
            result *= base;
        base *= base;
        n >>= 1;
    }
    result *= 9;
    result += 1;
    return result;
}
