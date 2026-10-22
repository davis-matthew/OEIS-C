/*
 * A006583  a(n) = Sum_{k=1..n-1} (k OR n-k).
 * https://oeis.org/A006583
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n-1} (k OR n-k)
 * Range:     u8 n <= 18, u16 n <= 277, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 1; k < n; k++) {
        result += ((seq_t)k) | ((seq_t)n - (seq_t)k);
    }
    
    return result;
}
