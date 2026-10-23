/*
 * A021082  Decimal expansion of 1/78.
 * https://oeis.org/A021082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (n==0)?0: {1,2,8,2,0,5}[(n-1)%6] – the digits of 1/78
 *            after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* digits of 1/78 repeat 1,2,8,2,0,5 after the initial 0 */
    static const seq_t block[6] = { (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)2, (seq_t)0, (seq_t)5 };
    return block[(n - 1) % 6];
}
