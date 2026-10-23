/*
 * A122461  Repetitions of even numbers four times.
 * https://oeis.org/A122461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor(n/4)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/4) using integer division of the unsigned index type */
    idx_t q = n / 4;
    /* Return the sequence value as seq_t */
    return (seq_t)q * 2;
}
