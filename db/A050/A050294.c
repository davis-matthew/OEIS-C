/*
 * A050294  Maximum cardinality of a 3-fold-free subset of {1, 2, ..., n}.
 * https://oeis.org/A050294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(floor(n/3)), a(0) = 0 (valid for n>=1, with offset
 *            1)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return (seq_t)n - a(n / 3);
}
