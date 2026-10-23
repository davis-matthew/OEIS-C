/*
 * A021035  Decimal expansion of 1/31.
 * https://oeis.org/A021035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit at position n in the repeating decimal expansion of
 *            1/31 The period is 15 digits: 0,3,2,2,5,8,0,6,4,5,1,6,1,2,9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char period[15] =
        {0,3,2,2,5,8,0,6,4,5,1,6,1,2,9};
    return (seq_t)period[n % 15];
}
