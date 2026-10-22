/*
 * A005204  Coding a recurrence.
 * https://oeis.org/A005204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1) << shift(n)) | a(n-3) for n >= 6, where shift(n)
 *            = shift(n-1) + shift(n-3) for n >= 9, with initial shifts:
 *            shift(6)=1, shift(7)=2, shift(8)=3. Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=4
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   get_shift, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/get_shift.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static bool *computed = NULL;
static idx_t computed_len = 0;

seq_t a(idx_t n) {
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        memo = grow_zero(memo, memo_len, new_len);
        memo_len = new_len;
    }
    
    if (n >= computed_len) {
        idx_t new_len = n + 1;
        bool *new_computed = (bool *)malloc(new_len * sizeof(bool));
        if (computed_len > 0) {
            memcpy(new_computed, computed, computed_len * sizeof(bool));
            free(computed);
        }
        memset(new_computed + computed_len, 0, (new_len - computed_len) * sizeof(bool));
        computed = new_computed;
        computed_len = new_len;
    }
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result;
    
    if (n < 6) {
        if (n < 3) result = 0;
        else if (n == 3) result = 1;
        else if (n == 4) result = 2;
        else result = 4;
    } else {
        seq_t a_n_minus_1 = a(n - 1);
        seq_t a_n_minus_3 = a(n - 3);
        idx_t shift = get_shift(n);
        
        result = (a_n_minus_1 << shift) | a_n_minus_3;
    }
    
    memo[n] = result;
    computed[n] = true;
    return result;
}
