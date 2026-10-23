/*
 * A132344  a(n) = n*2^(floor(n/2)).
 * https://oeis.org/A132344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(floor(n/2))
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) can be computed by right-shifting the index. */
    idx_t half = n >> 1;
    /* Compute 2^half via repeated left shifts of 1.  This avoids undefined
       behaviour for large half values on fixed-width types. */
    seq_t pow = 1;
    for (idx_t i = 0; i < half; ++i)
        pow <<= 1;
    /* Result is n multiplied by 2^half.  All operands are cast to seq_t first. */
    return ((seq_t)n) * pow;
}
