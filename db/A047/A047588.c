/*
 * A047588  Numbers that are congruent to {0, 1, 2, 3, 5, 6, 7} mod 8.
 * https://oeis.org/A047588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n-1)/7⌋ + offset[(n-1) mod 7] where offset =
 *            {0,1,2,3,5,6,7}. This implements the OEIS sequence A047588:
 *            numbers congruent to 0,1,2,3,5,6,7 (mod 8)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offsets of allowed numbers within each block of 8 */
    static const unsigned char offsets[7] = {0, 1, 2, 3, 5, 6, 7};

    /* block number (0‑based) and position within the block */
    idx_t block = (n - 1) / 7;
    idx_t pos   = (n - 1) % 7;

    /* compute result in seq_t keeping operations in the target type */
    seq_t result = (seq_t)block * 8 + offsets[pos];
    return result;
}
