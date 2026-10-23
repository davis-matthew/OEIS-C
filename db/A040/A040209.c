/*
 * A040209  Continued fraction for sqrt(224).
 * https://oeis.org/A040209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 14; a(n) = 28 if n even and n>0; a(n) = 1 otherwise
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)           /* offset term */
        return (seq_t)14;
    if ((n & 1U) == 0U)    /* even n */
        return (seq_t)28;
    return (seq_t)1;
}
