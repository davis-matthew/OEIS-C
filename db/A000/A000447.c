/*
 * A000447  a(n) = 1^2 + 3^2 + 5^2 + 7^2 + ... + (2*n-1)^2 = n*(4*n^2 - 1)/3.
 * https://oeis.org/A000447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n * (2*n - 1) * (2*n + 1) / 3
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    seq_t a = n_seq;
    seq_t b = 2 * n_seq - 1;
    seq_t c = 2 * n_seq + 1;
    
    if (a % 3 == 0) {
        a /= 3;
    } else if (b % 3 == 0) {
        b /= 3;
    } else {
        c /= 3;
    }
    
    return a * b * c;
}
