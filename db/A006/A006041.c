/*
 * A006041  a(n+1) = (n^2 - 1)*a(n) + n + 1.
 * https://oeis.org/A006041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = (n^2 - 1)*a(n) + n + 1; a(1) = 0, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    for (idx_t k = 1; k < n; k++) {
        seq_t k_seq = (seq_t)k;
        seq_t coeff = k_seq * k_seq - 1;
        result = coeff * result + k_seq + 1;
    }
    
    return result;
}
