/*
 * A007682  a(n) = -Sum_{k = 0..n-1} (n+k)!a(k)/(2k)!.
 * https://oeis.org/A007682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = -Sum_{k = 0..n-1} (n+k)!a(k)/(2k)!
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_size = 0;
static idx_t last_computed = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo_size = (n > 99) ? (n + 1) * 2 : 100;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        memo[0] = 1;
        last_computed = 0;
    }
    
    while (n >= memo_size) {
        memo_size *= 2;
        memo = (seq_t *)realloc(memo, memo_size * sizeof(seq_t));
    }
    
    for (idx_t m = last_computed + 1; m <= n; m++) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; k++) {
            seq_t ratio = 1;
            for (idx_t i = 2 * k + 1; i <= m + k; i++) {
                ratio *= (seq_t)i;
            }
            sum += ratio * memo[k];
        }
        memo[m] = -sum;
    }
    
    last_computed = (n > last_computed) ? n : last_computed;
    return memo[n];
}
