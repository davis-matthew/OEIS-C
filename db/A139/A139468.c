/*
 * A139468  a(n) = Sum_{k=0..n} C(n,3k+1)^2.
 * https://oeis.org/A139468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k\ge0} \binom{n}{3k+1}^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c   = 1;          // C(n,0)
    for (idx_t r = 1; r <= n; ++r) {
        c = (c * (seq_t)(n - r + 1)) / (seq_t)r;  // C(n,r) from C(n,r-1)
        if ((r % 3) == 1) {
            seq_t sq = c * c;
            sum += sq;
        }
    }
    return sum;
}
