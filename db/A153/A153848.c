/*
 * A153848  a(n) = 7*(10^(2*n) - 1)/33.
 * https://oeis.org/A153848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(10^(2*n) - 1)/33 (i.e., the number consisting of n
 *            copies of "21")
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * 100 + 21;
    return res;
}
