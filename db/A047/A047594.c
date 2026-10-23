/*
 * A047594  Numbers that are congruent to {0, 2, 3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-2)/7) for n >= 2, a(1) = 0
 * Range:     u8 n <= 224, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)            /* a(1) = 0 */
        return 0;

    /* For n >= 2, compute the required offset  */
    return (seq_t)n + (seq_t)((n - 2) / 7);
}
