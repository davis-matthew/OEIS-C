/*
 * A147758  Numbers whose binary representation is a palindrome formed from
 *          the reflected decimal expansion of the concatenation of 1, 0 and
 *          infinite digits 1.
 * https://oeis.org/A147758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=5; for n>=4: a(n) = 1<<(n-1) +
 *            ((((1<<(n-4))-1)<<2) + 1). This yields a binary palindrome of
 *            the form 1 0 (n-4)·'1' 0 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)5;

    /* n >= 4 */
    seq_t msb   = ( (seq_t)1 << (n-1) );                /* leading 1 at bit n-1   */
    seq_t block = ( ( (seq_t)1 << (n-4) ) - (seq_t)1 ); /* n-4 ones */
    block <<= 2;                                       /* shift to positions n-3 .. 2 */
    return msb + block + (seq_t)1;                     /* trailing 1 at LSB      */
}
