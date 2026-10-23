/*
 * A144459  a(n) = (3*n+1)*(5*n+1).
 * https://oeis.org/A144459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*(5*n+1)
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t1 = x * 3 + 1;
    seq_t t2 = x * 5 + 1;
    return t1 * t2;
}
