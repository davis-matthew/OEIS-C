/*
 * A004186  Arrange digits of n in decreasing order.
 * https://oeis.org/A004186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   arrange digits of n in decreasing order
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    // Extract digits
    unsigned char digits[20];  // enough for uint64_t
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = temp % 10;
        temp /= 10;
    }
    
    // Sort digits in decreasing order
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (digits[i] < digits[j]) {
                unsigned char t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }
    
    // Convert back to number
    seq_t result = 0;
    for (int i = 0; i < len; i++) {
        result = result * 10 + digits[i];
    }
    
    return result;
}
