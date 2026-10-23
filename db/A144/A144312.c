/*
 * A144312  a(n) = 5*n*(5*n + 1)/2.
 * https://oeis.org/A144312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(5*n + 1)/2
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = n;
    seq_t prod = 5 * tn;
    prod *= (5 * tn + 1);
    prod /= 2;
    return prod;
}
