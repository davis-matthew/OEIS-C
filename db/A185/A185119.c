/*
 * A185119  Number of connected 2-regular simple graphs on n vertices with
 *          girth at least 9.
 * https://oeis.org/A185119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n == 0 or n >= 9, 0 otherwise
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n < 9)
        return (seq_t)0;
    return (seq_t)1;
}
