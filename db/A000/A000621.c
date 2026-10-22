/*
 * A000621  Number of monosubstituted alkanes C(n-1)H(2n-1)-X with n-1 carbon
 *          atoms that are not stereoisomers.
 * https://oeis.org/A000621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   A(x) = 1 + x*A(x)*A(x^2), where A(x) is the offset 0 sequence *
 *            Recurrence: a(0) = 1, a(n) = sum_{j=0}^{floor((n-1)/2)}
 *            a(n-1-2j)*a(j) for n >= 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 45, u64 n <= 88, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    static idx_t max_computed = (idx_t)-1;
    
    idx_t target = n - 1;
    
    if (target >= memo_size) {
        idx_t new_size = (target + 1) * 2;
        seq_t *new_memo = (seq_t *)calloc(new_size, sizeof(seq_t));
        
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_size * sizeof(seq_t));
            free(memo);
        }
        
        memo = new_memo;
        memo_size = new_size;
    }
    
    if (max_computed == (idx_t)-1) {
        memo[0] = 1;
        max_computed = 0;
    }
    
    for (idx_t m = max_computed + 1; m <= target; ++m) {
        seq_t sum = 0;
        for (idx_t j = 0; j * 2 <= m - 1; ++j) {
            sum += memo[m - 1 - 2*j] * memo[j];
        }
        memo[m] = sum;
    }
    
    if (target > max_computed) {
        max_computed = target;
    }
    
    return memo[target];
}
