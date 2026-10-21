/*
 * A059153  a(n) = 2^(n+2)*(2^(n+1)-1).
 * https://oeis.org/A059153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+2)*(2^(n+1)-1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t two_n1 = (seq_t)1 << (n + 1);  /* 2^(n+1) */
    seq_t two_n2 = (seq_t)1 << (n + 2);  /* 2^(n+2) */
    return two_n2 * (two_n1 - 1);
}
