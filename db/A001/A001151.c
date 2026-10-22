/*
 * A001151  Describe the previous term! (method A - initial term is 8).
 * https://oeis.org/A001151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   Look-and-say sequence starting with 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cached_value = 0;
static idx_t cached_n = 0;

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)8;
    
    if (n == cached_n) return cached_value;
    
    seq_t current = (seq_t)8;
    idx_t start_i = 2;
    
    if (cached_n > 0 && cached_n < n) {
        current = cached_value;
        start_i = cached_n + 1;
    }
    
    for (idx_t i = start_i; i <= n; i++) {
        // Extract digits from current (least significant first)
        seq_t temp = current;
        int digits[10000];
        int digit_count = 0;
        
        if (temp == 0) {
            digits[0] = 0;
            digit_count = 1;
        } else {
            while (temp > 0) {
                digits[digit_count] = (int)(temp % (seq_t)10);
                temp = temp / (seq_t)10;
                digit_count++;
            }
        }
        
        // Reverse to get most significant first
        for (int j = 0; j < digit_count / 2; j++) {
            int tmp = digits[j];
            digits[j] = digits[digit_count - 1 - j];
            digits[digit_count - 1 - j] = tmp;
        }
        
        // Apply look-and-say transformation
        seq_t next = (seq_t)0;
        int j = 0;
        while (j < digit_count) {
            int digit = digits[j];
            int count = 1;
            while (j + count < digit_count && digits[j + count] == digit) {
                count++;
            }
            
            // Append count and digit to result
            next = next * (seq_t)10 + (seq_t)count;
            next = next * (seq_t)10 + (seq_t)digit;
            
            j += count;
        }
        
        current = next;
    }
    
    // Cache the result
    cached_n = n;
    cached_value = current;
    
    return current;
}
