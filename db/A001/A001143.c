/*
 * A001143  Describe the previous term! (method A - initial term is 6).
 * https://oeis.org/A001143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   describe the previous term using run-length encoding (count then
 *            digit for each run)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)6;
    }
    
    seq_t prev = a(n - 1);
    
    // Extract digits, most significant first
    seq_t digits[1000];
    int len = 0;
    
    seq_t x = prev;
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x > 0) {
            digits[len] = x % 10;
            x = x / 10;
            len++;
        }
        // Reverse to get most significant first
        for (int i = 0; i < len / 2; i++) {
            seq_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }
    
    // Encode runs: for each run of identical digits, output (count, digit)
    seq_t out[2000];
    int olen = 0;
    
    for (int i = 0; i < len; ) {
        seq_t digit = digits[i];
        int count = 1;
        while (i + count < len && digits[i + count] == digit) {
            count++;
        }
        
        // Append count as digits
        if (count >= 100) {
            out[olen++] = (seq_t)(count / 100);
            out[olen++] = (seq_t)((count / 10) % 10);
            out[olen++] = (seq_t)(count % 10);
        } else if (count >= 10) {
            out[olen++] = (seq_t)(count / 10);
            out[olen++] = (seq_t)(count % 10);
        } else {
            out[olen++] = (seq_t)count;
        }
        
        // Append the digit itself
        out[olen++] = digit;
        
        i += count;
    }
    
    // Convert output digits to result
    seq_t res = 0;
    for (int i = 0; i < olen; i++) {
        res = res * 10 + out[i];
    }
    
    return res;
}
