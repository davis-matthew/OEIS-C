/*
 * A003755  Number of spanning trees in S_4 X P_n.
 * https://oeis.org/A003755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*a(n-1) - 336*a(n-2) + 582*a(n-3) - 336*a(n-4) +
 *            48*a(n-5) - a(n-6) Base cases: a(1)=1, a(2)=54, a(3)=2240,
 *            a(4)=89964, a(5)=3596725, a(6)=143700480
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    
    if (n == 1) return 1;
    if (n == 2) return 54;
    if (n == 3) return 2240;
    if (n == 4) return 89964;
    if (n == 5) return (seq_t)3596725;
    if (n == 6) return (seq_t)143700480;
    
    if (cache == NULL) {
        cache_len = (n < 100) ? 100 : n + 50;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
        if (cache == NULL) return 0;
        
        cache[1] = 1;
        cache[2] = 54;
        cache[3] = 2240;
        cache[4] = 89964;
        cache[5] = (seq_t)3596725;
        cache[6] = (seq_t)143700480;
        
        for (idx_t i = 7; i <= cache_len; i++) {
            cache[i] = 0;
        }
    } else if (cache_len < n) {
        idx_t old_len = cache_len;
        cache_len = n + 50;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
        if (new_cache == NULL) return 0;
        
        for (idx_t i = 1; i <= old_len; i++) {
            new_cache[i] = cache[i];
        }
        for (idx_t i = old_len + 1; i <= cache_len; i++) {
            new_cache[i] = 0;
        }
        
        free(cache);
        cache = new_cache;
    }
    
    for (idx_t i = 7; i <= n; i++) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)48 * cache[i-1]
                     - (seq_t)336 * cache[i-2]
                     + (seq_t)582 * cache[i-3]
                     - (seq_t)336 * cache[i-4]
                     + (seq_t)48 * cache[i-5]
                     - cache[i-6];
        }
    }
    
    return cache[n];
}
