/*
 * A209280  First difference of A050289 = numbers whose digits are a
 *          permutation of (1,...,9).
 * https://oeis.org/A209280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A209280 – First difference of all 9‑digit permutations of 1…9
    a(n) = P(n+1) - P(n)   where P(k) is the k‑th permutation of 123456789
    in increasing numeric order (equivalently lexicographic order).   */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool next_permutation_int(int *arr, int n) {
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (arr[j] <= arr[i]) j--;
    int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) { tmp = arr[l]; arr[l] = arr[r]; arr[r] = tmp; l++; r--; }
    return true;
}

seq_t a(idx_t n) {
    /* static cache of differences, computed once upon first call */
    static seq_t *diff = NULL;
    static const idx_t N = 362880;       /* 9!   */
    if (!diff) {
        diff = (seq_t *)malloc((N - 1) * sizeof(seq_t));
        if (!diff) return 0;             /* out of memory */
        int digits[9] = {1,2,3,4,5,6,7,8,9};
        seq_t prev = 0, curr = 0;
        /* numeric value of the first (lexicographically smallest) permutation */
        for (int k = 0; k < 9; k++) prev = prev * 10 + digits[k];
        idx_t idx = 0;
        while (next_permutation_int(digits, 9)) {
            curr = 0;
            for (int k = 0; k < 9; k++) curr = curr * 10 + digits[k];
            diff[idx++] = curr - prev;
            prev = curr;
        }
        /* idx should equal N-1, the array is now full */
    }
    if (n < 1 || n >= N) return 0; /* out of defined range */
    return diff[n - 1];
}
