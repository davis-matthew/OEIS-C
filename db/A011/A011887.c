/*
 * A011887  a(n) = floor( n*(n-1)*(n-2)/5 ).
 * https://oeis.org/A011887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/5 )
 * Range:     u8 n <= 11, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    return ((seq_t)n * (seq_t)(n-1) * (seq_t)(n-2)) / (seq_t)5;
}
