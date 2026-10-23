/*
 * A016966  a(n) = (6*n + 4)^10.
 * https://oeis.org/A016966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 13, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)4;
    seq_t res = base;
    for (int i = 1; i < 10; ++i) {
        res *= base;
    }
    return res;
}
