/*
 * A005214  Triangular numbers together with squares (excluding 0).
 * https://oeis.org/A005214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Merge of triangular numbers k*(k+1)/2 and perfect squares k^2,
 *            removing duplicates
 * Range:     u8 n <= 33, u16 n <= 611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t tri_k = 1;
    idx_t sq_k = 1;
    
    for (idx_t i = 0; i < n - 1; i++) {
        seq_t tri = (seq_t)tri_k * ((seq_t)tri_k + 1) / 2;
        seq_t sq = (seq_t)sq_k * (seq_t)sq_k;
        
        if (tri < sq) {
            tri_k++;
        } else if (sq < tri) {
            sq_k++;
        } else {
            tri_k++;
            sq_k++;
        }
    }
    
    seq_t tri = (seq_t)tri_k * ((seq_t)tri_k + 1) / 2;
    seq_t sq = (seq_t)sq_k * (seq_t)sq_k;
    
    return tri < sq ? tri : sq;
}
