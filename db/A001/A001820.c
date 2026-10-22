/*
 * A001820  Central factorial numbers: 2nd subdiagonal of A008955.
 * https://oeis.org/A001820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^2 + 6*n + 5)*a(n-1) - (n^2 + n + 1)*(3*n^2 + 3*n +
 *            1)*a(n-2) + n^6*a(n-3), a(0) = 1, a(1) = 14, a(2) = 273
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_cap = 0;

static void ensure_capacity(idx_t n) {
    if (n >= memo_cap) {
        idx_t new_cap = (n + 1000) * 2;
        seq_t *new_memo = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (memo) {
            for (idx_t i = 0; i < memo_cap; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        for (idx_t i = memo_cap; i < new_cap; i++) {
            new_memo[i] = 0;
        }
        memo = new_memo;
        memo_cap = new_cap;
    }
}

static void initialize_memo(void) {
    if (memo_cap == 0) {
        ensure_capacity(2);
        memo[0] = 1;
        memo[1] = 14;
        memo[2] = 273;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 14;
    if (n == 2) return 273;
    
    initialize_memo();
    ensure_capacity(n);
    
    if (memo[n] != 0) {
        return memo[n];
    }
    
    for (idx_t i = 3; i <= n; i++) {
        if (memo[i] == 0) {
            seq_t ni = (seq_t)i;
            seq_t ni2 = ni * ni;
            seq_t ni6 = ni2 * ni2 * ni2;
            
            seq_t c1 = 3 * ni2 + 6 * ni + 5;
            seq_t c2a = ni2 + ni + 1;
            seq_t c2b = 3 * ni2 + 3 * ni + 1;
            seq_t c2 = c2a * c2b;
            
            memo[i] = c1 * memo[i-1] - c2 * memo[i-2] + ni6 * memo[i-3];
        }
    }
    
    return memo[n];
}
