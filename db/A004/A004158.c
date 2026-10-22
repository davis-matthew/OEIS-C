/*
 * A004158  Triangular numbers written backwards.
 * https://oeis.org/A004158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A004086(A000217(n))
 * Range:     u8 n <= 13, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    
    seq_t result = 0;
    seq_t temp = tri;
    
    while (temp > 0) {
        result = result * 10 + (temp % 10);
        temp /= 10;
    }
    
    return result;
}
