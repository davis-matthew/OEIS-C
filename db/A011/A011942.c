/*
 * A011942  a(n) = floor( n*(n-1)*(n-2)*(n-3)/32 ).
 * https://oeis.org/A011942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)*(n-3)/32 )
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 288, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the product contains a zero factor, so the result is 0. */
    if (n < 4) return 0;

    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1) * (nn - 2) * (nn - 3);
    /* Divide by 32 exactly by shifting right 5 bits (floor). */
    return prod >> 5;
}
