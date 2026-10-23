/*
 * A040872  Continued fraction for sqrt(903).
 * https://oeis.org/A040872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(903): a(0)=30; 
   for n ≥ 1: a(n)=20 if n is odd, otherwise 60. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;
    return (n & 1U) ? (seq_t)20 : (seq_t)60;
}
