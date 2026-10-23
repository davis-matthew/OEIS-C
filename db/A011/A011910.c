/*
 * A011910  a(n) = floor( n*(n-1)*(n-2)/28 ).
 * https://oeis.org/A011910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/28)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t nn   = (seq_t)n;
    seq_t one  = (seq_t)(n - 1);
    seq_t two  = (seq_t)(n - 2);
    seq_t prod = nn * one * two;
    return prod / (seq_t)28;
}
