/*
 * A005256  Number of weighted voting procedures.
 * https://oeis.org/A005256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) - a(floor((n-3)/2)), a(1) = 1, a(2) = 3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1000));
        max_computed = 0;
    } else if (n > max_computed) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1000));
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 3;
        } else {
            idx_t idx_to_subtract = (i - 3) / 2;
            seq_t val_to_subtract = (idx_to_subtract >= 1) ? cache[idx_to_subtract] : 0;
            cache[i] = 2 * cache[i - 1] - val_to_subtract;
        }
    }
    
    max_computed = n;
    return cache[n];
}
