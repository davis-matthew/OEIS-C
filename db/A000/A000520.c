/*
 * A000520  Nearest integer to log_10(n).
 * https://oeis.org/A000520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to log_10(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t n_sq = n * n;
    seq_t k = 0;
    idx_t threshold = 10;
    
    while (n_sq >= threshold) {
        k++;
        threshold *= 100;
    }
    
    return k;
}
