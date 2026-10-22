/*
 * A005991  a(n) is the index of the last occurrence of the n-th distinct term
 *          (equal to n+1) in sequence A155934(m) = k(m) = the least integer
 *          such that every m X m (0,1)-matrix with exactly k(m) ones in each
 *          row and in each column contains a 2 X 2 submatrix without zeros.
 * https://oeis.org/A005991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*(n+1) + max(0, n-5)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t base = n_seq * (n_seq + 1);
    
    if (n > 5) {
        base += n_seq - 5;
    }
    
    return base;
}
