/*
 * A004718  The Danish composer Per Nørgård's "infinity sequence", invented in
 *          an attempt to unify in a perfect way repetition and variation:
 *          a(2n) = -a(n), a(2n+1) = a(n) + 1, a(0) = 0.
 * https://oeis.org/A004718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy, hear
 * Formula:   binary interpretation: start at 0, read bits MSB-LSB, +1 for
 *            1-bit, negate for 0-bit
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t result = 0;
    int msb = -1;
    idx_t temp = n;
    while (temp > 0) {
        msb++;
        temp >>= 1;
    }
    
    for (int i = msb; i >= 0; i--) {
        if (n & (1ULL << i)) {
            result += 1;
        } else {
            result = -result;
        }
    }
    
    return result;
}
