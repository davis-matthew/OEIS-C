/*
 * A001145  Describe the previous term! (method A - initial term is 7).
 * https://oeis.org/A001145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n+1) describes a(n) by counting consecutive identical digits
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 7;
    
    seq_t current = 7;
    
    for (idx_t i = 2; i <= n; i++) {
        // Extract digits from current
        seq_t temp = current;
        int digits[2000];
        int digit_count = 0;
        
        while (temp > 0 && digit_count < 2000) {
            digits[digit_count++] = (int)(temp % 10);
            temp /= 10;
        }
        
        // Reverse to get digits in normal order
        for (int j = 0; j < digit_count / 2; j++) {
            int tmp = digits[j];
            digits[j] = digits[digit_count - 1 - j];
            digits[digit_count - 1 - j] = tmp;
        }
        
        // Build next number by counting consecutive runs
        seq_t next = 0;
        int j = 0;
        while (j < digit_count) {
            int digit = digits[j];
            int count = 1;
            while (j + count < digit_count && digits[j + count] == digit) {
                count++;
            }
            
            // Append count and digit to next
            next = next * 10 + count;
            next = next * 10 + digit;
            
            j += count;
        }
        
        current = next;
    }
    
    return current;
}
