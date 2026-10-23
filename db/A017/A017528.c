/*
 * A017528  a(n) = (12*n)^8.
 * https://oeis.org/A017528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 21, u128 n <= 5461,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 12;
    seq_t res = 1;
    for (idx_t i = 0; i < 8; ++i)
        res *= m;
    return res;
}
