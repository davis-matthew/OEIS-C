/*
 * A085036  a(n) = (5*n+2)*(5*n+7).
 * https://oeis.org/A085036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+2)*(5*n+7)
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1  = sn * (seq_t)5 + (seq_t)2;
    seq_t t2  = sn * (seq_t)5 + (seq_t)7;
    return t1 * t2;
}
