/*
 * A002942  a(n) = n^2 written backwards.
 * https://oeis.org/A002942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n^2 written backwards
 * Range:     u8 n <= 11, u16 n <= 102, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t reversed = 0;
    seq_t temp = n_sq;
    
    while (temp > 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }
    
    return reversed;
}
