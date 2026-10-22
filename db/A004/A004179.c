/*
 * A004179  Omit 4's from n.
 * https://oeis.org/A004179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Omit all digit 4's from the decimal representation of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        
    idx_t temp = n;
    idx_t digits[20];
    int len = 0;
    
    while (temp > 0) {
        digits[len++] = temp % 10;
        temp /= 10;
    }
    
    seq_t result = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (digits[i] != 4) {
            result = result * 10 + (seq_t)digits[i];
        }
    }
    
    return result;
}
