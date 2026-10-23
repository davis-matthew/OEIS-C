/*
 * A047448  Numbers that are congruent to {0, 2, 3, 5, 6} mod 8.
 * https://oeis.org/A047448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n−1)/5⌋ + {0,2,3,5,6}[ (n−1) mod 5 ] sequence of
 *            numbers congruent to {0,2,3,5,6} modulo 8
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t m = (n - 1) / 5;
    unsigned r = (unsigned)((n - 1) % 5);

    static const seq_t prefix[5] = {0, 2, 3, 5, 6};

    seq_t base = (seq_t)m * 8U;
    return base + prefix[r];
}
