/*
 * A081032  Positions of black keys on piano keyboard, starting with A0 = the
 *          1st key.
 * https://oeis.org/A081032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = floor((12*n + 2)/5)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    return ((seq_t)n * 12 + 2) / 5;
}
