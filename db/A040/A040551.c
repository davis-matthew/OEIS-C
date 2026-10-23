/*
 * A040551  Continued fraction for sqrt(575).
 * https://oeis.org/A040551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; a(n)=46 if n>0 and n even; a(n)=1 otherwise
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)23;
    return ( (n & 1) == 0 ) ? (seq_t)46 : (seq_t)1;
}
