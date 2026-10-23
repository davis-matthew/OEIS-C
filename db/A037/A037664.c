/*
 * A037664  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0.
 * https://oeis.org/A037664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=1}^{n} ((k-1)%3==0?3:((k-1)%3==1?1:0)) * 8^{n-k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char pattern[3] = {3, 1, 0};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * 8 + pattern[i % 3];
    return res;
}
