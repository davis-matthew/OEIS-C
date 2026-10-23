/*
 * A040960  Continued fraction for sqrt(992).
 * https://oeis.org/A040960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction of sqrt(992):
 * a(0) = 31, and for n ≥ 1 the terms alternate 2 and 62:
 * a(n) = (n is odd) ? 2 : 62
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)31;
    /* use bitwise test for odd/even to avoid division */
    if ((n & (idx_t)1))
        return (seq_t)2;
    return (seq_t)62;
}
