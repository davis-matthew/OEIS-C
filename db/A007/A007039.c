/*
 * A007039  Number of cyclic binary n-bit strings with no alternating
 *          substring of length > 2.
 * https://oeis.org/A007039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) for n >= 5; a(1)=2, a(2)=2,
 *            a(3)=2, a(4)=6
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 0) return 0;
    
    if (n == 1) return 2;
    if (n == 2) return 2;
    if (n == 3) return 2;
    if (n == 4) return 6;
    
    static idx_t memo_capacity = 0;
    static idx_t memo_len = 0;
    static seq_t *memo = NULL;
    
    if (memo == NULL) {
        memo_capacity = 16;
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_capacity);
        memo[1] = 2;
        memo[2] = 2;
        memo[3] = 2;
        memo[4] = 6;
        memo_len = 4;
    }
    
    while (n >= memo_capacity) {
        idx_t new_capacity = memo_capacity * 2;
        seq_t *new_memo = (seq_t *)malloc(sizeof(seq_t) * new_capacity);
        for (idx_t i = 1; i <= memo_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        memo_capacity = new_capacity;
    }
    
    while (memo_len < n) {
        idx_t i = memo_len + 1;
        memo[i] = 2 * memo[i-1] - memo[i-2] + memo[i-4];
        memo_len++;
    }
    
    return memo[n];
}
