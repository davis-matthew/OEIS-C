/*
 * A007479  Earliest sequence with a(a(a(n))) = 2n.
 * https://oeis.org/A007479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(0) = 0, a(2*n) = 2*a(n), a(6*n+1) = 6*n+3, a(6*n+3) = 6*n+5,
 *            a(6*n+5) = 12*n+2
 * Range:     u8 n <= 127, u16 n <= 9059, u32 n <= 9059, u64 n <= 9059,
 *            u128 n <= 9059, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    
    if (n % 2 == 0) {
        return 2 * a(n / 2);
    }
    
    seq_t n_seq = (seq_t)n;
    idx_t r = n % 6;
    
    if (r == 1 || r == 3) {
        return n_seq + 2;
    } else {
        return 2 * n_seq - 8;
    }
}
