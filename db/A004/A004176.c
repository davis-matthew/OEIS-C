/*
 * A004176  Omit 1's from n.
 * https://oeis.org/A004176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   Omit all '1' digits from decimal representation of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;
    }
    
    idx_t temp = n;
    int digits[20];
    int idx = 0;
    
    while (temp > 0) {
        int digit = temp % 10;
        if (digit != 1) {
            digits[idx++] = digit;
        }
        temp /= 10;
    }
    
    if (idx == 0) {
        return 0;
    }
    
    for (int i = 0; i < idx / 2; i++) {
        int t = digits[i];
        digits[i] = digits[idx - 1 - i];
        digits[idx - 1 - i] = t;
    }
    
    seq_t result = 0;
    for (int i = 0; i < idx; i++) {
        result = result * 10 + digits[i];
    }
    
    return result;
}
