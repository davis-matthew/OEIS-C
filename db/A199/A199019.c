/*
 * A199019  a(n) = 3*11^n - 1.
 * https://oeis.org/A199019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*11^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 11;
    idx_t e = n;

    while (e > 0)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }

    result *= 3;
    result -= 1;

    return result;
}
