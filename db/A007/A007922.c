/*
 * A007922  Smallest k such that k!! is a multiple of n.
 * https://oeis.org/A007922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find smallest k such that k!! is divisible by n */
seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t odd_df = 1;
    seq_t even_df = 1;
    
    if (odd_df % n_val == 0) {
        return 1;
    }
    
    for (seq_t k = 2; k <= n_val; k++) {
        if (k % 2 == 0) {
            even_df = (even_df * k) % n_val;
            if (even_df == 0) {
                return k;
            }
        } else {
            odd_df = (odd_df * k) % n_val;
            if (odd_df == 0) {
                return k;
            }
        }
    }
    
    return n_val;
}
