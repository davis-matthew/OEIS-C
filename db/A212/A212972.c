/*
 * A212972  Number of triples (w,x,y) with all terms in {0,...,n} and w >=
 *          floor((x+y)/3).
 * https://oeis.org/A212972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 8*n^2 + 10*n + 4 - (((n+1) mod 3) mod 2)) / 3
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = (seq_t)2 * nseq * nseq * nseq
               + (seq_t)8 * nseq * nseq
               + (seq_t)10 * nseq
               + (seq_t)4;
    idx_t mod3 = (n + 1) % 3;
    idx_t sub    = mod3 % 2;
    term -= (seq_t)sub;
    return term / 3;
}
