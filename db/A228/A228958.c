/*
 * A228958  a(n) = 1*2 + 3*4 + 5*6 + 7*8 + 9*10 + 11*12 + 13*14 + ... + (up to
 *          n).
 * https://oeis.org/A228958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} ((2*k-1)*(2*k)) + (n % 2 ? n : 0)
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t pairs = n >> 1; /* floor(n/2) */
    for (idx_t k = 1; k <= pairs; ++k) {
        seq_t odd  = (seq_t)(2 * k - 1);
        seq_t even = (seq_t)(2 * k);
        res += odd * even;
    }
    if (n & 1) {
        res += (seq_t)n;
    }
    return res;
}
