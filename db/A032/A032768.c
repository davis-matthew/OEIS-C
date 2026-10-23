/*
 * A032768  a(n) = floor( n(n+1)(n+2)(n+3)(n+4) / (n+(n+1)+(n+2)+(n+3)+(n+4))
 *          ).
 * https://oeis.org/A032768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n+1)*(n+3)*(n+4) / 5 )
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 268, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x1 = x + 1;
    seq_t x3 = x + 3;
    seq_t x4 = x + 4;

    seq_t prod = x * x1;
    prod       = prod * x3;
    prod       = prod * x4;

    return prod / 5;
}
