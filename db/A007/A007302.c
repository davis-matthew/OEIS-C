/*
 * A007302  Optimal cost function between two processors at distance n.
 * https://oeis.org/A007302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = hammingweight(XOR(n, 3*n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t xor_result = n ^ (3 * n);
    
    seq_t count = 0;
    while (xor_result) {
        xor_result &= xor_result - 1;
        count++;
    }
    return count;
}
