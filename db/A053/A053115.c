/*
 * A053115  a(n) = ((8*n+10)(!^8))/20, related to A034908 ((8*n+2)(!^8) octo-
 *          or 8-factorials).
 * https://oeis.org/A053115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((8*n+10)(!^8))/20 Recurrence: a(0) = 1; a(n) = a(n-1) *
 *            (8*n + 10) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= n; ++i)
        val *= ((seq_t)8 * i + (seq_t)10);
    return val;
}
