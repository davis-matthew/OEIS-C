/*
 * A017183  a(n) = (9*n + 1)^11.
 * https://oeis.org/A017183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)9 * n + (seq_t)1;
    seq_t res  = (seq_t)1;
    for (int i = 0; i < 11; ++i)
        res *= base;
    return res;
}
