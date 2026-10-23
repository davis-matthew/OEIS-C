/*
 * A017049  a(n) = (7*n + 5)^9.
 * https://oeis.org/A017049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 19, u128 n <= 2729,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * n + 5;
    seq_t result = base;
    for (int i = 1; i < 9; ++i) {
        result *= base;
    }
    return result;
}
