/*
 * A017304  a(n) = (10*n + 2)^12.
 * https://oeis.org/A017304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 3, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 10 + 2;
    seq_t res  = 1;
    for (int i = 0; i < 12; ++i) {
        res *= base;
    }
    return res;
}
