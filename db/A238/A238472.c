/*
 * A238472  a(n) = binomial(5*n+7, 4)/5 for n >= 0.
 * https://oeis.org/A238472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n+7, 4)/5
 * Range:     u8 none, u16 n <= 2, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t1 = 5*x + 7;
    seq_t t2 = 5*x + 6;
    seq_t t3 = 5*x + 5;
    seq_t t4 = 5*x + 4;
    seq_t prod = t1 * t2;
    prod = prod * t3;
    prod = prod * t4;
    return prod / 120;
}
