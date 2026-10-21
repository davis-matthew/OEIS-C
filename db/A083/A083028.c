/*
 * A083028  Numbers that are congruent to {0, 2, 3, 5, 7, 8, 11} mod 12.
 * https://oeis.org/A083028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*⌊(n-1)/7⌋ + R where R = {0,2,3,5,7,8,11} indexed by
 *            (n-1) mod 7. a(1) = 0, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 7,
 *            a(6) = 8, a(7) = 11, a(8) = 12,
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int residues[7] = {0, 2, 3, 5, 7, 8, 11};
    idx_t block = (n - 1) / 7;
    int r = residues[(n - 1) % 7];
    /* Use seq_t for intermediate to support arbitrary-precision builds. */
    return (seq_t)block * 12 + (seq_t)r;
}
