/*
 * A037688  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0,3.
 * https://oeis.org/A037688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=1..n} d_k * 4^{n-k} where d_k cycles 1,2,0,3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned pattern[4] = { 1, 2, 0, 3 };
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
        val = val * 4 + (seq_t)pattern[(unsigned)(i & 3)];
    return val;
}
