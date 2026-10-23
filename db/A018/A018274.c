/*
 * A018274  Divisors of 78.
 * https://oeis.org/A018274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth smallest divisor of 78
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1 */

    const idx_t LIMIT = 78;      /* the number whose divisors we list */

    idx_t count = 0;
    for (idx_t d = 1; d <= LIMIT; ++d)
    {
        if (LIMIT % d == 0)      /* d is a divisor of 78 */
        {
            ++count;
            if (count == n)
                return (seq_t)d; /* return the nth divisor */
        }
    }

    return 0;                   /* n is larger than the number of divisors */
}
