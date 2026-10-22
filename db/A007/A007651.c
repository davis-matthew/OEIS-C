/*
 * A007651  Describe the previous term! (method B - initial term is 1).
 * https://oeis.org/A007651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = run-length description of previous term, starting from
 *            a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    
    seq_t current = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t temp = current;
        seq_t digits[100000];
        int len = 0;
        
        if (temp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (temp > 0) {
                digits[len++] = temp % 10;
                temp /= 10;
            }
        }
        
        // Reverse to get most significant digit first
        for (int j = 0; j < len / 2; j++) {
            seq_t t = digits[j];
            digits[j] = digits[len - 1 - j];
            digits[len - 1 - j] = t;
        }
        
        // Encode runs: for each run, emit digit then count
        seq_t result[100000];
        int res_len = 0;
        
        int j = 0;
        while (j < len) {
            seq_t digit = digits[j];
            int run_len = 1;
            
            while (j + run_len < len && digits[j + run_len] == digit) {
                run_len++;
            }
            
            result[res_len++] = digit;
            
            // Append run length as decimal digits
            int c = run_len;
            seq_t c_digits[20];
            int c_len = 0;
            
            if (c == 0) {
                c_digits[0] = 0;
                c_len = 1;
            } else {
                while (c > 0) {
                    c_digits[c_len++] = c % 10;
                    c /= 10;
                }
            }
            
            for (int k = c_len - 1; k >= 0; k--) {
                result[res_len++] = c_digits[k];
            }
            
            j += run_len;
        }
        
        // Convert result back to seq_t
        current = 0;
        for (int j = 0; j < res_len; j++) {
            current = current * 10 + result[j];
        }
    }
    
    return current;
}
