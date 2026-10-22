/*
 * A006015  Nim product 2*n.
 * https://oeis.org/A006015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   Convert n to quaternary, apply digit transformation (0→0, 1→2,
 *            2→3, 3→1), convert back to decimal
 * Range:     u8 n <= 255, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const unsigned char transform[4] = {0, 2, 3, 1};
    seq_t result = 0;
    seq_t power = 1;
    
    while (n > 0) {
        unsigned int digit = n & 3;
        result += (seq_t)transform[digit] * power;
        power *= 4;
        n >>= 2;
    }
    
    return result;
}
