/*
 * A211545  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+x+y>0.
 * https://oeis.org/A211545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (8*n*n - 3*n + 3) / 2 (valid for n ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 818, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt   = (seq_t)n;
    seq_t n_sq = nt * nt;
    seq_t part = (seq_t)8 * n_sq - (seq_t)3 * nt + (seq_t)3;
    seq_t res  = nt * part / (seq_t)2;
    return res;
}
