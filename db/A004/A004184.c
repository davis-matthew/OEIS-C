/*
 * A004184  Omit 9's from n.
 * https://oeis.org/A004184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = result of removing all 9's from the decimal
 *            representation of n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    int digits[20]; // Enough for uint64_t
    int len = 0;
    idx_t temp = n;
    
    while (temp > 0) {
        int digit = temp % 10;
        if (digit != 9) {
            digits[len++] = digit;
        }
        temp /= 10;
    }
    
    seq_t result = 0;
    for (int i = len - 1; i >= 0; i--) {
        result = result * 10 + (seq_t)digits[i];
    }
    
    return result;
}
