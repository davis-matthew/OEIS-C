/*
 * A131800  Period 4: repeat [1, 2, 5, 6].
 * https://oeis.org/A131800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1, 2, 5, 6}[n mod 4]
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use bitwise AND for modulo 4 (since 4 is a power of two). */
    idx_t r = n & 3;
    /* Static constant array of the period. */
    static const seq_t period[4] = {1, 2, 5, 6};
    return period[r];
}
