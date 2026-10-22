/*
 * A007495  Josephus problem: survivors.
 * https://oeis.org/A007495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Let t(k,n) = (t(k-1,n) + n) mod k and t(1,n) = 0; then a(n) =
 *            t(n,n) + 1
 * Range:     u8 n <= 263, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = 0;
    seq_t n_val = (seq_t)n;
    
    for (seq_t k = 2; k <= n_val; k++) {
        t = (t + n_val % k) % k;
    }
    
    return t + 1;
}
