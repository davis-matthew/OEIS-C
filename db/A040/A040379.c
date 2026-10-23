/*
 * A040379  Continued fraction for sqrt(399).
 * https://oeis.org/A040379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 19, a(n) = 38 if n > 0 and even, a(n) = 1 if n odd.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;
    if ((n & 1) == 0)
        return (seq_t)38;
    return (seq_t)1;
}
