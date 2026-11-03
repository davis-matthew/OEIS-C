/*
 * A007927  Some nontrivial permutation of digits is a factorial number.
 * https://oeis.org/A007927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

static seq_t digits_to_num(int *digits, int len) {
    seq_t result = 0;
    for (int i = len - 1; i >= 0; i--) {
        result = result * 10 + digits[i];
    }
    return result;
}

static int get_digits(seq_t num, int *digits) {
    int len = 0;
    if (num == 0) {
        digits[0] = 0;
        return 1;
    }
    seq_t temp = num;
    while (temp > 0) {
        digits[len++] = (int)(temp % 10);
        temp /= 10;
    }
    return len;
}

static bool is_factorial(seq_t num) {
    seq_t f = 1;
    if (f == num) return true;
    for (int i = 1; i <= 100; i++) {
        f *= i;
        if (f == num) return true;
        if (f > num) break;
    }
    return false;
}

static bool next_permutation(int *arr, int len) {
    int i = len - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    
    int j = len - 1;
    while (j > i && arr[j] <= arr[i]) j--;
    
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    
    for (int a = i + 1, b = len - 1; a < b; a++, b--) {
        temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }
    return true;
}

static bool has_factorial_permutation(seq_t num) {
    int digits[20];
    int len = get_digits(num, digits);
    
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (digits[i] > digits[j]) {
                int temp = digits[i];
                digits[i] = digits[j];
                digits[j] = temp;
            }
        }
    }
    
    do {
        seq_t perm = digits_to_num(digits, len);
        if (perm != num && is_factorial(perm)) {
            return true;
        }
    } while (next_permutation(digits, len));
    
    return false;
}

/* Sieve: iterate through candidates checking if any digit permutation is a factorial */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 1;
    
    while (count < n) {
        if (has_factorial_permutation(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
    
    return 0;
}
