/*
 * A040675  Continued fraction for sqrt(702).
 * https://oeis.org/A040675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=26, a(n)=2 if n%2==1 else 52 for n>0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;
    else if ((n & 1) == 1) /* odd index */
        return (seq_t)2;
    else                    /* even index >0 */
        return (seq_t)52;
}
