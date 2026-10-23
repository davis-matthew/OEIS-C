/*
 * A184017  a(n) = n-1+ceiling((n/3)^2); complement of A184016.
 * https://oeis.org/A184017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceil(n^2 / 9)
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for zero in OEIS offset, but guard */
    seq_t sq      = (seq_t)n * (seq_t)n;
    seq_t ceil_div = (sq + 8) / 9;    /* ceil(n^2 / 9) using integer arithmetic */
    return (seq_t)n - 1 + ceil_div;
}
