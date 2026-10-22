/*
 * A005536  a(0) = 0; thereafter a(2n) = n - a(n) - a(n-1), a(2n+1) = n -
 *          2a(n) + 1.
 * https://oeis.org/A005536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0; thereafter a(2n) = n - a(n) - a(n-1), a(2n+1) = n -
 *            2a(n) + 1
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_len = 0;

static void ensure_memo(idx_t n) {
    if (n >= memo_len) {
        idx_t new_len = memo_len == 0 ? 1 : memo_len * 2;
        while (new_len <= n) new_len *= 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        bool *new_computed = (bool *)malloc(new_len * sizeof(bool));
        memset(new_computed, false, new_len * sizeof(bool));
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            memcpy(new_computed, computed, memo_len * sizeof(bool));
            free(memo);
            free(computed);
        }
        memo = new_memo;
        computed = new_computed;
        memo_len = new_len;
    }
}

static seq_t compute_a(idx_t n) {
    if (n == 0) return 0;
    
    ensure_memo(n);
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result;
    if (n % 2 == 0) {
        idx_t m = n / 2;
        result = (seq_t)m - compute_a(m) - compute_a(m - 1);
    } else {
        idx_t m = (n - 1) / 2;
        result = (seq_t)m - 2 * compute_a(m) + 1;
    }
    
    computed[n] = true;
    memo[n] = result;
    return result;
}

seq_t a(idx_t n) {
    return compute_a(n);
}
