/*
 * A005132  Recamán's sequence (or Recaman's sequence): a(0) = 0; for n > 0,
 *          a(n) = a(n-1) - n if nonnegative and not already in the sequence,
 *          otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A005132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, hear, look
 * Formula:   a(n) = a(n-1) - n if nonnegative and not already in sequence,
 *            otherwise a(n) = a(n-1) + n; a(0) = 0
 * Range:     u8 n <= 100, u16 n <= 9387, u32 n <= 9387, u64 n <= 9387,
 *            u128 n <= 9387, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n < cache_len) {
        return cache[n];
    }
    
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache_len = 1;
    } else {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }
    
    for (idx_t i = cache_len; i <= n; i++) {
        seq_t prev = cache[i - 1];
        seq_t i_seq = (seq_t)i;
        
        if (prev >= i_seq) {
            seq_t candidate = prev - i_seq;
            
            bool found = false;
            for (idx_t j = 0; j < i; j++) {
                if (cache[j] == candidate) {
                    found = true;
                    break;
                }
            }
            cache[i] = found ? (prev + i_seq) : candidate;
        } else {
            cache[i] = prev + i_seq;
        }
    }
    
    cache_len = n + 1;
    return cache[n];
}
