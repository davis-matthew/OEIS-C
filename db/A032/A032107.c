/*
 * A032107  Number of reversible strings with n labeled beads of 2 colors.
 * https://oeis.org/A032107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) * n! for n > 1 * a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is a special case */
    if (n == 1)
        return (seq_t)2;

    seq_t result = 1;

    /* multiply by 2^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i)
        result *= (seq_t)2;

    /* multiply by n! (starting from 2 because 1! = 1) */
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;

    return result;
}
