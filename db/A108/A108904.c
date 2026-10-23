/*
 * A108904  a(n) has two outer digits 9 and n inner digits 7.
 * https://oeis.org/A108904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = '9' followed by n copies of '7' and a final '9'
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 9;
    for (idx_t i = 0; i < n; ++i)
        res = res * 10 + 7;
    res = res * 10 + 9;
    return res;
}
