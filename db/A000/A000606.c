/*
 * A000606  Number of nonnegative solutions to x^2 + y^2 + z^2 <= n.
 * https://oeis.org/A000606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative solutions to x^2 + y^2 + z^2 <= n
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    seq_t x, y, z;
    
    for (x = 0; x * x <= n_seq; x++) {
        seq_t x2 = x * x;
        for (y = 0; x2 + y * y <= n_seq; y++) {
            seq_t xy2 = x2 + y * y;
            for (z = 0; xy2 + z * z <= n_seq; z++) {
                count++;
            }
        }
    }
    
    return count;
}
