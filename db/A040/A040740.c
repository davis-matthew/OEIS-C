/*
 * A040740  Continued fraction for sqrt(768).
 * https://oeis.org/A040740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=27; a(n)=pattern[(n-1)%8] for n>=1 where
 *            pattern={1,2,2,13,2,2,1,54}
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 27;
    static const seq_t pattern[8] = {1,2,2,13,2,2,1,54};
    return pattern[(n-1) & 7];
}
