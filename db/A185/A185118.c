/*
 * A185118  Number of connected 2-regular simple graphs on n vertices with
 *          girth at least 8.
 * https://oeis.org/A185118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=0 for 0<n<8; a(n)=1 for n>=8
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
    if (n < 8)
        return (seq_t)0;
    return (seq_t)1;
}
