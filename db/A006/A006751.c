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
 * Helpers:   compute_next, digits_to_seq, seq_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/compute_next.h"
#include "../utils/digits_to_seq.h"
#include "../utils/seq_to_digits.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)2;
    }
    
    if (memo == NULL) {
        memo_len = (n > 100) ? n : 100;
        memo = (seq_t *)calloc(memo_len + 1, sizeof(seq_t));
        memo[1] = (seq_t)2;
    } else if (n > memo_len) {
        idx_t new_len = n * 2;
        seq_t *new_memo = (seq_t *)calloc(new_len + 1, sizeof(seq_t));
        memcpy(new_memo, memo, (memo_len + 1) * sizeof(seq_t));
        free(memo);
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
