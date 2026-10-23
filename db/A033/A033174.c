/*
 * A033174  Number of days in n years (n=1 is the first leap year).
 * https://oeis.org/A033174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1461*n+3)/4)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast idx_t to seq_t for correct arithmetic on the target type */
    seq_t nn = (seq_t)n;
    return (nn * 1461 + 3) / 4;
}
