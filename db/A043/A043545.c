/*
 * A043545  (Maximal base-2 digit of n) - (minimal base-2 digit of n).
 * https://oeis.org/A043545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if n+1 is a power of two, otherwise 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n + 1;
    return ((t & (t - 1)) == 0) ? 0 : 1;
}
