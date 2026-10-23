/*
 * A184534  a(n) = floor(1/{(4+n^3)^(1/3)}), where {}=fractional part.
 * https://oeis.org/A184534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n*n/4) for n >= 4, with a(1)=1, a(2)=3, a(3)=7
 * Range:     u8 n <= 15, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
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
        return (seq_t)7;
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2 */
    return (3 * nn) / 4;              /* floor of (3*n^2)/4 */
}
