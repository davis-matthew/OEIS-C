/*
 * A007931  Numbers that contain only 1's and 2's. Nonempty binary strings of
 *          length n in lexicographic order.
 * https://oeis.org/A007931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   To get a(n), write n+1 in base 2, remove initial 1, add 1 to all
 *            remaining digits
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n + 1;
    
    // Find the highest bit position
    int highest_bit = -1;
    idx_t temp = m;
    while (temp > 0) {
        highest_bit++;
        temp >>= 1;
    }
    
    // Build result from bits below the highest bit
    seq_t result = 0;
    for (int i = highest_bit - 1; i >= 0; i--) {
        result = result * 10 + (((m >> i) & 1) + 1);
    }
    
    return result;
}
