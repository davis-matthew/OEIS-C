/*
 * A003605  Unique monotonic sequence of nonnegative integers satisfying
 *          a(a(n)) = 3n.
 * https://oeis.org/A003605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3^k + n if n < 2*3^k; a(n) = 3n - 3^(k+1) if n >= 2*3^k,
 *            where k = floor(log_3(n))
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t n_seq = (seq_t)n;
    seq_t p = 1;
    
    /* Find largest power of 3 <= n */
    while (n_seq / 3 >= p) {
        p *= 3;
    }
    
    seq_t two_p = p + p;
    
    if (n_seq < two_p) {
        return p + n_seq;
    } else {
        return n_seq * 3 - p * 3;
    }
}
