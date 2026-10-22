/*
 * A007604  a(n) = a(n-1) + a(n-1-(number of odd terms so far)).
 * https://oeis.org/A007604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + a(max(1, n-1 - (count of odd terms in a(1), ...,
 *            a(n-1)))), a(1) = 1
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 117, u64 n <= 320,
 *            u128 n <= 903, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;
static idx_t odd_count = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (cache == NULL) {
        cache_len = 32;
        if (n > cache_len) {
            cache_len = 1;
            while (cache_len < n) cache_len *= 2;
        }
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
    } else if (n > cache_len) {
        idx_t new_len = cache_len * 2;
        while (new_len < n) new_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        if (i == 1) {
            cache[0] = 1;
            odd_count = 1;
        } else {
            idx_t m = ((i - 1) > odd_count) ? ((i - 1) - odd_count) : 1;
            seq_t new_val = cache[i - 2] + cache[m - 1];
            cache[i - 1] = new_val;
            if (new_val & 1) odd_count++;
        }
    }
    
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    
    return cache[n - 1];
}
