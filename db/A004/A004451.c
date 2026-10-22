/*
 * A004451  Nimsum n + 10.
 * https://oeis.org/A004451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2(-1)^floor(n/2) + 8(-1)^floor(n/8)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n;
    
    // Add 2*(-1)^floor(n/2)
    if ((n / 2) & 1) {
        result -= 2;
    } else {
        result += 2;
    }
    
    // Add 8*(-1)^floor(n/8)
    if ((n / 8) & 1) {
        result -= 8;
    } else {
        result += 8;
    }
    
    return result;
}
