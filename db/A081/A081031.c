/*
 * A081031  Positions of white keys on piano keyboard, starting with A0 = the
 *          1st key.
 * https://oeis.org/A081031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = floor((12*n-3)/7)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (12*n - 3) / 7 with integer arithmetic.
       All operands are positive for n >= 1, so the usual integer division
       yields floor((12*n-3)/7). */
    return ((seq_t)n * 12 - (seq_t)3) / 7;
}
