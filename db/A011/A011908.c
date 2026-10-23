/*
 * A011908  a(n) = floor( n*(n-1)*(n-2)/26 ).
 * https://oeis.org/A011908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/26 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1796, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3u)
        return 0;

    /* Convert the unsigned index to the sequence type */
    seq_t x0 = (seq_t)n;
    seq_t x1 = (seq_t)(n - 1u);
    seq_t x2 = (seq_t)(n - 2u);

    /* Compute the product n*(n-1)*(n-2) */
    seq_t prod = x0 * x1;
    prod *= x2;

    /* Divide by 26 (exact division for the given formula) */
    return prod / (seq_t)26;
}
