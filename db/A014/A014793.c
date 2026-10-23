/*
 * A014793  Squares of odd octagonal numbers.
 * https://oeis.org/A014793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((1 + 2*n)*(1 + 6*n))^2
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 73, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t1 = (seq_t)1 + ((seq_t)2 * n);
    seq_t t2 = (seq_t)1 + ((seq_t)6 * n);
    seq_t prod = t1 * t2;
    return prod * prod;
}
