/*
 * A006751  Describe the previous term! (method A - initial term is 2).
 * https://oeis.org/A006751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   Look-and-say sequence with frequency-digit notation. a(n+1)
 *            describes a(n) by listing (frequency, digit) pairs for
 *            consecutive identical digits read left to right. a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t digits_to_seq(const char *digits, idx_t len) {
    seq_t result = 0;
    for (idx_t i = 0; i < len; i++) {
        result = result * 10 + (seq_t)(digits[i] - '0');
    }
    return result;
}

static char *seq_to_digits(seq_t val, idx_t *len_out) {
    if (val == 0) {
        char *result = (char *)malloc(2);
        result[0] = '0';
        result[1] = '\0';
        *len_out = 1;
        return result;
    }
    
    seq_t temp = val;
    idx_t len = 0;
    while (temp > 0) {
        len++;
        temp /= 10;
    }
    
    char *result = (char *)malloc(len + 1);
    result[len] = '\0';
    temp = val;
    for (idx_t i = len; i > 0; i--) {
        result[i - 1] = '0' + (char)(temp % 10);
        temp /= 10;
    }
    
    *len_out = len;
    return result;
}

static seq_t compute_next(seq_t current) {
    idx_t digit_len = 0;
    char *digits = seq_to_digits(current, &digit_len);
    
    char *result = (char *)malloc(digit_len * 2 + 1);
    idx_t result_len = 0;
    
    idx_t i = 0;
    while (i < digit_len) {
        char current_digit = digits[i];
        idx_t count = 1;
        while (i + count < digit_len && digits[i + count] == current_digit) {
            count++;
        }
        
        result[result_len++] = '0' + (char)count;
        result[result_len++] = current_digit;
        
        i += count;
    }
    result[result_len] = '\0';
    
    seq_t res = digits_to_seq(result, result_len);
    
    free(digits);
    free(result);
    
    return res;
}

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)2;
    }
    
    if (memo == NULL) {
        memo_len = (n > 100) ? n : 100;
        memo = mem_new(seq_t, memo_len + 1);
        memo[1] = (seq_t)2;
    } else if (n > memo_len) {
        idx_t new_len = n * 2;
        seq_t *new_memo = mem_new(seq_t, new_len + 1);
        mem_copy(new_memo, memo, (memo_len + 1) * sizeof(seq_t));
        mem_free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    for (idx_t i = 2; i <= n; i++) {
        if (memo[i] == 0) {
            memo[i] = compute_next(memo[i - 1]);
        }
    }
    
    return memo[n];
}
