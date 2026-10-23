/*
 * A047359  Numbers that are congruent to {0, 2, 3} mod 7.
 * https://oeis.org/A047359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*⌊(n-1)/3⌋ + [0,2,3][(n-1) mod 3]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Index must be >= 1 according to OEIS offset. */
    idx_t cycle = (n - 1) / 3;
    uint8_t pos   = (uint8_t)((n - 1) % 3);
    static const seq_t offsets[3] = {0, 2, 3};

    return (seq_t)cycle * 7 + offsets[pos];
}
