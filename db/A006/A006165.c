/*
 * A006165  a(1) = a(2) = 1; thereafter a(2n+1) = a(n+1) + a(n), a(2n) =
 *          2a(n).
 * https://oeis.org/A006165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; a(2n) = 2a(n); a(2n+1) = a(n+1) + a(n)
 * Range:     u8 n <= 383, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    while (memo_len <= n) {
        idx_t new_len = (memo_len == 0) ? 100 : (memo_len * 2);
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        memset(new_memo + memo_len, 0, (new_len - memo_len) * sizeof(seq_t));
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (memo[n] != 0) return memo[n];
    
    seq_t result;
    if (n == 1 || n == 2) {
        result = 1;
    } else if (n % 2 == 0) {
        result = 2 * a(n / 2);
    } else {
        idx_t k = n / 2;
        result = a(k + 1) + a(k);
    }
    
    memo[n] = result;
    return result;
}
