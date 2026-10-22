/*
 * A004447  Nimsum n + 6.
 * https://oeis.org/A004447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2*(-1)^floor(n/2) + 4*(-1)^floor(n/4)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n;
    
    int sign1 = 1 - 2 * ((n / 2) & 1);
    result += 2 * sign1;
    
    int sign2 = 1 - 2 * ((n / 4) & 1);
    result += 4 * sign2;
    
    return result;
}
