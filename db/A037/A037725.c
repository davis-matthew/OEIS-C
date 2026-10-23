/*
 * A037725  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3,1.
 * https://oeis.org/A037725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = base-6 number formed by the repeating digits 2,0,3,1,
 *            using the first n digits of the repetition. That is, a(n) =
 *            sum_{k=0}^{n-1} (pattern[k mod 4]) * 6^{n-1-k}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const uint8_t pattern[4] = {2, 0, 3, 1};
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * 6 + (seq_t)pattern[i & 3];
    }
    return result;
}
