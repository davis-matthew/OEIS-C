/*
 * A002243  Numbers that are not the sum of 3 distinct triangular numbers.
 * https://oeis.org/A002243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = n-th positive integer that cannot be expressed as the sum
 *            of 3 distinct triangular numbers
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   is_sum_of_three_distinct_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_sum_of_three_distinct_triangular.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t count_computed = 0;
    
    if (n > 0 && n <= count_computed) {
        return cache[n - 1];
    }
    
    if (n <= 0) return 0;
    
    idx_t num = (count_computed > 0) ? (idx_t)cache[count_computed - 1] : 0;
    
    if (n > cache_cap) {
        idx_t new_cap = (n < 100) ? 100 : (n + 50);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;
        
        for (idx_t i = 0; i < count_computed; i++) {
            new_cache[i] = cache[i];
        }
        
        if (cache) free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    while (count_computed < n && num < 100000) {
        num++;
        if (!is_sum_of_three_distinct_triangular(num)) {
            count_computed++;
            cache[count_computed - 1] = (seq_t)num;
        }
    }
    
    if (count_computed >= n) {
        return cache[n - 1];
    }
    return 0;
}
