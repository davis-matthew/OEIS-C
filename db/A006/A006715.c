/*
 * A006715  Describe the previous term! (method A - initial term is 3).
 * https://oeis.org/A006715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   look-and-say sequence with a(1) = 3; a(n+1) obtained by reading
 *            digits of a(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t build_from_digits(int *digits, idx_t len) {
    seq_t result = 0;
    for (idx_t i = 0; i < len; i++) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}

static idx_t extract_digits(seq_t num, int *digits) {
    if (num == 0) {
        digits[0] = 0;
        return 1;
    }
    
    idx_t count = 0;
    seq_t temp = num;
    
    while (temp > 0) {
        digits[count++] = (int)(temp % 10);
        temp /= 10;
    }
    
    for (idx_t i = 0; i < count / 2; i++) {
        int t = digits[i];
        digits[i] = digits[count - 1 - i];
        digits[count - 1 - i] = t;
    }
    
    return count;
}

static seq_t look_and_say(seq_t prev) {
    int in_digits[8000];
    idx_t in_len = extract_digits(prev, in_digits);
    
    int out_digits[16000];
    idx_t out_len = 0;
    
    idx_t i = 0;
    while (i < in_len && out_len < 16000) {
        int digit = in_digits[i];
        idx_t count = 1;
        
        while (i + count < in_len && in_digits[i + count] == digit) {
            count++;
        }
        
        if (count < 10) {
            out_digits[out_len++] = (int)count;
        } else {
            int cd[20];
            idx_t cdlen = 0;
            seq_t c = (seq_t)count;
            while (c > 0) {
                cd[cdlen++] = (int)(c % 10);
                c /= 10;
            }
            for (idx_t j = 0; j < cdlen && out_len < 16000; j++) {
                out_digits[out_len++] = cd[cdlen - 1 - j];
            }
        }
        
        if (out_len < 16000) {
            out_digits[out_len++] = digit;
        }
        
        i += count;
    }
    
    return build_from_digits(out_digits, out_len);
}

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)3;
    }
    
    seq_t current = (seq_t)3;
    for (idx_t i = 2; i <= n; i++) {
        current = look_and_say(current);
    }
    
    return current;
}
