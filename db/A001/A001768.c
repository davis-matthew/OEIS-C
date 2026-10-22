/*
 * A001768  Sorting numbers: number of comparisons for merge insertion sort of
 *          n elements.
 * https://oeis.org/A001768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n(z-1) - floor((2^(z+2) - 3z) / 6), where z =
 *            floor(log_2(3n+3))
 * Range:     u8 n <= 41, u16 n <= 5880, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    // Compute z = floor(log_2(3n+3))
    idx_t arg = 3 * n + 3;
    int z = 0;
    idx_t temp = arg;
    while (temp > 1) {
        temp >>= 1;
        z++;
    }
    
    // Compute a(n) = n(z-1) - floor((2^(z+2) - 3z) / 6)
    seq_t z_seq = (seq_t)z;
    seq_t result = n_seq * (z_seq - 1);
    
    // Compute 2^(z+2)
    seq_t pow2 = (seq_t)1 << (z + 2);
    
    // Compute floor((2^(z+2) - 3z) / 6)
    seq_t numerator = pow2 - 3 * z_seq;
    seq_t term = numerator / 6;
    
    return result - term;
}
