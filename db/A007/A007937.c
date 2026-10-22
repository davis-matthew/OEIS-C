/*
 * A007937  Nonsquares such that some permutation of digits is a square.
 * https://oeis.org/A007937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* A007937: Nonsquares with some digit permutation being a square (search method) */

static uint64_t isqrt(uint64_t x) {
    if (x == 0) return 0;
    uint64_t root = x;
    while (true) {
        uint64_t new_root = (root + x / root) / 2;
        if (new_root >= root) return root;
        root = new_root;
    }
}

static bool is_square(uint64_t x) {
    uint64_t root = isqrt(x);
    return root * root == x;
}

static int get_digits(uint64_t x, int *digits) {
    if (x == 0) {
        digits[0] = 0;
        return 1;
    }
    int len = 0;
    while (x > 0) {
        digits[len++] = x % 10;
        x /= 10;
    }
    return len;
}

static int cmp_int(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

static bool next_permutation(int *arr, int len) {
    int i = len - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    if (i < 0) return false;
    
    int j = len - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }
    
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    
    int left = i + 1;
    int right = len - 1;
    while (left < right) {
        temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
    
    return true;
}

static bool has_square_permutation(uint64_t x) {
    int digits[20];
    int len = get_digits(x, digits);
    
    qsort(digits, len, sizeof(int), cmp_int);
    
    do {
        uint64_t num = 0;
        for (int i = 0; i < len; i++) {
            num = num * 10 + digits[i];
        }
        if (is_square(num)) {
            return true;
        }
    } while (next_permutation(digits, len));
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 1;
    
    while (true) {
        if (!is_square((uint64_t)candidate) && has_square_permutation((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate++;
    }
}
