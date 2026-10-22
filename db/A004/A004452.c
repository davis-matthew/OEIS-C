/*
 * A004452  Nimsum n + 11.
 * https://oeis.org/A004452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (-1)^n + 2*(-1)^floor(n/2) + 8*(-1)^floor(n/8)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n;
    
    if ((n & 1) == 0) {
        result += 1;
    } else {
        result -= 1;
    }
    
    if (((n >> 1) & 1) == 0) {
        result += 2;
    } else {
        result -= 2;
    }
    
    if (((n >> 3) & 1) == 0) {
        result += 8;
    } else {
        result -= 8;
    }
    
    return result;
}
