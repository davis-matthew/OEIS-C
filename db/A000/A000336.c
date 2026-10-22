/*
 * A000336  a(n) = a(n-1)*a(n-2)*a(n-3)*a(n-4); for n < 5, a(n) = n.
 * https://oeis.org/A000336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2)*a(n-3)*a(n-4); for n < 5, a(n) = n
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_size = 0;

static void ensure_memo(idx_t n) {
    if (n < memo_size) return;
    
    idx_t new_size = n + 1;
    if (new_size < 1000) new_size = 1000;
    
    seq_t *new_memo = (seq_t *)malloc(sizeof(seq_t) * new_size);
    bool *new_computed = (bool *)malloc(sizeof(bool) * new_size);
    
    if (memo) {
        for (idx_t i = 0; i < memo_size; i++) {
            new_memo[i] = memo[i];
            new_computed[i] = computed[i];
        }
        free(memo);
        free(computed);
    }
    
    for (idx_t i = memo_size; i < new_size; i++) {
        new_computed[i] = false;
    }
    
    memo = new_memo;
    computed = new_computed;
    memo_size = new_size;
}

seq_t a(idx_t n) {
    if (n < 5) {
        return (seq_t)n;
    }
    
    ensure_memo(n);
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result = a(n-1) * a(n-2) * a(n-3) * a(n-4);
    memo[n] = result;
    computed[n] = true;
    
    return result;
}
