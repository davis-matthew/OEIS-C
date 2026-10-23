/*
 * A198686  a(n) = 4*7^n-1.
 * https://oeis.org/A198686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*7^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)7;
    return pow * (seq_t)4 - (seq_t)1;
}
