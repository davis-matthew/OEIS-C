/*
 * A017565  a(n) = (12*n + 3)^9.
 * https://oeis.org/A017565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 11, u128 n <= 1592,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 12 + 3;
    seq_t res  = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        res *= base;
    }
    return res;
}
