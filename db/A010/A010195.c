/*
 * A010195  Continued fraction for sqrt(136).
 * https://oeis.org/A010195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=11; for n>0, a(n) = (n%4==0 ? 22 : 1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    return (n % (idx_t)4 == 0) ? (seq_t)22 : (seq_t)1;
}
