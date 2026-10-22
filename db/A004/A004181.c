/*
 * A004181  Omit 6's from n.
 * https://oeis.org/A004181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n with all 6's removed from decimal representation
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t digits[20]; // Max 20 digits for uint64_t
    int count = 0;
    idx_t temp = n;
    
    while (temp > 0) {
        int digit = (int)(temp % 10);
        if (digit != 6) {
            digits[count++] = (seq_t)digit;
        }
        temp /= 10;
    }
    
    // Reconstruct number from filtered digits in reverse order
    seq_t result = 0;
    for (int i = count - 1; i >= 0; i--) {
        result = result * 10 + digits[i];
    }
    
    return result;
}
