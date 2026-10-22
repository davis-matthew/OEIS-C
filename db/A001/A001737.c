/*
 * A001737  Squares written in base 2.
 * https://oeis.org/A001737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007088(A000290(n)), where A000290(n) = n^2 and
 *            A007088(k) = k written in base 2 (binary digits interpreted as
 *            decimal)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * ((seq_t)n);
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (n_sq > 0) {
        seq_t bit = n_sq & 1;
        result = result + bit * multiplier;
        multiplier = multiplier * 10;
        n_sq = n_sq >> 1;
    }
    
    return result;
}
