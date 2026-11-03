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
 * Validated: standard, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static idx_t get_shift(idx_t n) {
    if (n < 6) return 0;
    if (n == 6) return 1;
    if (n == 7) return 2;
    if (n == 8) return 3;
    
    idx_t s6 = 1, s7 = 2, s8 = 3;
    for (idx_t i = 9; i <= n; i++) {
        idx_t next = s6 + s8;
        s6 = s7;
        s7 = s8;
        s8 = next;
    }
    return s8;
}

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
