/*
 * A005418  Number of (n-1)-bead black-white reversible strings; also binary
 *          grids; also row sums of Losanitsch's triangle A034851; also number
 *          of caterpillar graphs on n+2 vertices.
 * https://oeis.org/A005418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2^(n-2) + 2^(floor(n/2) - 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    
    seq_t term1 = (seq_t)1 << (n - 2);
    seq_t term2 = (seq_t)1 << (n / 2 - 1);
    
    return term1 + term2;
}
