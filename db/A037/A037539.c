/*
 * A037539  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,1.
 * https://oeis.org/A037539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + d_{n-1}, where d_{n-1} repeats 1,2,1; a(0)=0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[3] = {(seq_t)1, (seq_t)2, (seq_t)1};
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * 6 + pat[i % 3];
    }
    return result;
}
