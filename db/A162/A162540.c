/*
 * A162540  a(n) = (2*n+1)*(2*n+3)*(2*n+5)/3.
 * https://oeis.org/A162540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(2*n+3)*(2*n+5)/3
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 1169, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t1 = 2 * ns + 1;
    seq_t t2 = 2 * ns + 3;
    seq_t t3 = 2 * ns + 5;
    seq_t prod = t1 * t2;
    prod = prod * t3;
    return prod / (seq_t)3;
}
