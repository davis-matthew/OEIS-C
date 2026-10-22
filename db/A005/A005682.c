/*
 * A005682  Number of Twopins positions.
 * https://oeis.org/A005682
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-4) - a(n-6), with a(5)=1, a(6)=2, a(7)=4,
 *            a(8)=8, a(9)=15, a(10)=28
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_filled = 0;

seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    if (cache == NULL) {
        cache_len = 11;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[5] = 1;
        cache[6] = 2;
        cache[7] = 4;
        cache[8] = 8;
        cache[9] = 15;
        cache[10] = 28;
        cache_filled = 10;
    }
    
    while (cache_filled < n) {
        if (cache_filled + 1 >= cache_len) {
            cache_len = cache_len * 2;
            cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        }
        cache_filled++;
        cache[cache_filled] = 2 * cache[cache_filled - 1] - cache[cache_filled - 4] - cache[cache_filled - 6];
    }
    
    return cache[n];
}
