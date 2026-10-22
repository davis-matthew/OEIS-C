/*
 * A004185  Arrange digits of n in increasing order, then (for n > 0) omit the
 *          zeros.
 * https://oeis.org/A004185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy, look
 * Formula:   Arrange digits of n in increasing order, then (for n > 0) omit
 *            the zeros
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    char digits[21];
    int len = 0;
    idx_t temp = n;
    
    while (temp > 0) {
        digits[len++] = '0' + (temp % 10);
        temp /= 10;
    }
    
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (digits[j] > digits[j + 1]) {
                char t = digits[j];
                digits[j] = digits[j + 1];
                digits[j + 1] = t;
            }
        }
    }
    
    seq_t result = 0;
    for (int i = 0; i < len; i++) {
        if (digits[i] != '0' || result != 0) {
            result = result * 10 + (digits[i] - '0');
        }
    }
    
    return result;
}
