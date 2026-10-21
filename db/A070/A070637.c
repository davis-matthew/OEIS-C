/*
 * A070637  a(n) = n^6 mod 14.
 * https://oeis.org/A070637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 14 (periodic with period 14)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n mod 14 completely determines n^6 mod 14, we use a lookup table. */
    static const seq_t table[14] = {
        0, 1, 8, 1, 8, 1, 8, 7,
        8, 1, 8, 1, 8, 1
    };
    idx_t r = n % 14;
    return table[r];
}
