/*
 * A176448  a(n) = 7*2^n - 2.
 * https://oeis.org/A176448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n - 2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        p = p + p;          /* p = 2 * p  -> 2^k */
    seq_t res = (seq_t)7 * p - (seq_t)2;
    return res;
}
