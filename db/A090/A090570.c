/*
 * A090570  Numbers that are congruent to {0, 1} mod 9.
 * https://oeis.org/A090570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)/2 - (8 if n even else 4)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Calculate 9*n in seq_t to avoid overflow in idx_t multiplication */
    seq_t prod = (seq_t)n * 9;
    seq_t half = prod / 2;                /* floor division, truncates */
    if ((n & 1) == 0)                     /* even n  */
        return half - 8;
    else                                  /* odd n   */
        return half - 4;
}
