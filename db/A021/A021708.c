/*
 * A021708  Decimal expansion of 1/704.
 * https://oeis.org/A021708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of decimal expansion of 1/704. For n < 6: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=4, a(4)=2, a(5)=0. For n >= 6: the digits
 *            alternate 4 and 5, so a(n) = (n & 1) ? 5 : 4
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {0, 0, 1, 4, 2, 0};

    if (n < 6)
        return init[n];

    /* For n >= 6, digits alternate 4,5,4,5,... */
    return (n & 1) ? (seq_t)5 : (seq_t)4;
}
