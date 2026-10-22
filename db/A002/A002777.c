/*
 * A002777  Restricted permutations.
 * https://oeis.org/A002777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*a(n-1) + 2*(n-d)*a(n-e) where (d,e)=(2,3) if n
 *            even, (1,2) if n odd Base cases: a(0)=1, a(1)=a(2)=a(3)=0,
 *            a(4)=4
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_capacity = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n >= 1 && n <= 3) return 0;
    if (n == 4) return 4;
    
    if (memo == NULL) {
        memo_capacity = (n < 256) ? 256 : n + 256;
        memo = (seq_t *)calloc(memo_capacity, sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 0;
        memo[2] = 0;
        memo[3] = 0;
        memo[4] = 4;
        computed_up_to = 4;
    }
    
    if (memo_capacity <= n) {
        idx_t newcap = n + 256;
        memo = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
        for (idx_t i = memo_capacity; i < newcap; i++) {
            memo[i] = 0;
        }
        memo_capacity = newcap;
    }
    
    for (idx_t k = computed_up_to + 1; k <= n; k++) {
        seq_t term1 = (seq_t)(k - 1) * memo[k - 1];
        seq_t term2;
        
        if (k % 2 == 0) {
            term2 = 2 * (seq_t)(k - 2) * memo[k - 3];
        } else {
            term2 = 2 * (seq_t)(k - 1) * memo[k - 2];
        }
        
        memo[k] = term1 + term2;
    }
    
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    
    return memo[n];
}
