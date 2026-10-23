/*
 * A018263  Divisors of 52.
 * https://oeis.org/A018263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 52 in ascending order
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is outside the number of divisors, return 0. */
    if (n == 0)
        return 0;
    const idx_t limit = 52;          /* number to factor */
    idx_t count = 0;
    for (idx_t d = 1; d <= limit; ++d)
    {
        if (limit % d == 0)
        {
            ++count;
            if (count == n)
                return (seq_t)d;
        }
    }
    return 0;
}
