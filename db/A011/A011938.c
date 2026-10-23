/*
 * A011938  a(n) = floor( n*(n-1)*(n-2)*(n-3)/28 ).
 * https://oeis.org/A011938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)*(n-3)/28 )
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 370, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;
    seq_t n_t = (seq_t)n;
    seq_t prod = n_t * (n_t - 1) * (n_t - 2) * (n_t - 3);
    return prod / 28;
}
