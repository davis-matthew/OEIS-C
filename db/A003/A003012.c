/*
 * A003012  Maximum number of cubes of side length 2 that can be packed into a
 *          3-dimensional torus of side length 2*n+1.
 * https://oeis.org/A003012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^3 + n^2 - n for n <= 3; a(n) = n^3 + n^2 - 3n + 13 for
 *            n >= 4
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;
    seq_t n3 = n2 * nseq;
    
    if (nseq <= 3) {
        return n3 + n2 - nseq;
    }
    
    return n3 + n2 - 3 * nseq + 13;
}
