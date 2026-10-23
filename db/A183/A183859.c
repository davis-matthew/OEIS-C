/*
 * A183859  a(n) = n - 1 + ceiling((n^2)/3); complement of A183858.
 * https://oeis.org/A183859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceiling((n^2)/3)
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t ceil_div = (nn + 2) / 3;
    seq_t result = (seq_t)n - 1 + ceil_div;
    return result;
}
