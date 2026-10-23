/*
 * A033418  a(n) = floor(98/n).
 * https://oeis.org/A033418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = floor(98/n) */

seq_t a(idx_t n)
{
    /* For n >= 1, integer division already yields the floor for positive
       values. For n == 0 we return 0 arbitrarily, though the OEIS offset
       starts at 1. */
    if (n == 0)
        return 0;
    return (seq_t)98 / (seq_t)n;
}
