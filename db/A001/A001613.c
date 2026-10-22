/*
 * A001613  Delete all odd digits from n.
 * https://oeis.org/A001613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n with all odd digits removed
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned char digits[64];
    int len = 0;
    idx_t temp = n;
    
    // Extract digits from least significant to most
    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0 && len < 64) {
            digits[len++] = temp % 10;
            temp /= 10;
        }
    }
    
    // Build result keeping only even digits, in order from most to least significant
    seq_t result = 0;
    int has_digit = 0;
    
    for (int i = len - 1; i >= 0; i--) {
        if (digits[i] % 2 == 0) {
            result = result * 10 + (seq_t)digits[i];
            has_digit = 1;
        }
    }
    
    return has_digit ? result : 0;
}
