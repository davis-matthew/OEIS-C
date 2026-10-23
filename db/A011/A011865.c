/*
 * A011865  a(n) = floor( n*(n - 1)/12 ).
 * https://oeis.org/A011865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n - 1)/12 )
 * Range:     u8 n <= 55, u16 n <= 887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula yields 0, and
       (seq_t)n - 1 would underflow, so handle small n explicitly. */
    if (n < 2) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    return (nn * (nn - 1)) / 12;
}
