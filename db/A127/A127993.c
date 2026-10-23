/*
 * A127993  Minimum bowling score for a game with n strikes.
 * https://oeis.org/A127993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 10*n for n <= 7; a(8)=90; a(n) = 60*n - 420 for n >= 9
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    if (n <= 7)
        return tn * (seq_t)10;
    if (n == 8)
        return (seq_t)90;
    return tn * (seq_t)60 - (seq_t)420;
}
