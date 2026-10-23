/*
 * A011858  a(n) = floor( n*(n-1)/5 ).
 * https://oeis.org/A011858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)/5 )
 * Range:     u8 n <= 36, u16 n <= 572, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n*(n-1) safely in seq_t and divide by 5. */
    return ((seq_t)n * ((seq_t)n - 1)) / 5;
}
