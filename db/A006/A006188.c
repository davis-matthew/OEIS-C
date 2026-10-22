/*
 * A006188  Maximal cycles in trivalent graphs.
 * https://oeis.org/A006188
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) - a(n-4) for n >= 7, with base cases a(3)=7,
 *            a(4)=15, a(5)=29, a(6)=57
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static bool *computed = NULL;
    static idx_t memo_size = 0;
    
    if (n <= 6) {
        if (n == 3) return 7;
        if (n == 4) return 15;
        if (n == 5) return 29;
        if (n == 6) return 57;
        return 0;
    }
    
    // Ensure memo arrays are large enough
    if (n >= memo_size) {
        idx_t new_size = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        bool *new_computed = (bool *)malloc(new_size * sizeof(bool));
        
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_size * sizeof(seq_t));
            free(memo);
        }
        if (computed != NULL) {
            memcpy(new_computed, computed, memo_size * sizeof(bool));
            free(computed);
        }
        
        memset(new_computed + memo_size, 0, (new_size - memo_size) * sizeof(bool));
        
        memo = new_memo;
        computed = new_computed;
        memo_size = new_size;
    }
    
    if (computed[n]) return memo[n];
    
    memo[n] = 4 * a(n - 2) - a(n - 4);
    computed[n] = true;
    return memo[n];
}
