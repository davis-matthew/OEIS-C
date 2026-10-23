/*
 * A158399  a(n) = 784*n - 1.
 * https://oeis.org/A158399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 784*n - 1
 * Range:     u8 none, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)784 * n;
    result -= (seq_t)1;
    return result;
}
