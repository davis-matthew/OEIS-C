/*
 * A002541  a(n) = Sum_{k=1..n-1} floor((n-k)/k).
 * https://oeis.org/A002541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{i=2..n} floor(n/i)
 * Range:     u8 n <= 73, u16 n <= 8043, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t n_val = (seq_t)n;
    
    for (idx_t i = 2; i <= n; i++) {
        result += n_val / (seq_t)i;
    }
    return result;
}
