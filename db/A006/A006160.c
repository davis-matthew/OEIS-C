/*
 * A006160  a(n)=a(a(n-5))+a(n-a(n-5)).
 * https://oeis.org/A006160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-5)) + a(n-a(n-5)), with a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void ensure_capacity(idx_t n) {
    if (n >= memo_len) {
        idx_t new_len = (n < 1000) ? 1000 : (n + n / 10 + 100);
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
        }
        memo = new_memo;
        memo_len = new_len;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    ensure_capacity(n);
    
    if (n <= 5) {
        memo[n - 1] = 1;
        return 1;
    }
    
    if (memo[n - 1] != 0) {
        return memo[n - 1];
    }
    
    seq_t a_n_minus_5 = a(n - 5);
    seq_t result = a((idx_t)a_n_minus_5) + a(n - (idx_t)a_n_minus_5);
    memo[n - 1] = result;
    return result;
}
