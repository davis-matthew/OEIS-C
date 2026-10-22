/*
 * A000423  a(n) is smallest number > a(n-1) of form a(i)*a(j), i < j < n.
 * https://oeis.org/A000423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is smallest number > a(n-1) of form a(i)*a(j), i < j < n,
 *            with a(1) = 2, a(2) = 3. The sequence contains 2, 3 and all
 *            numbers of the form 2^a * 3^b where a >= 1, b >= 1
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    // Ensure cache is large enough
    if (n >= cache_size) {
        idx_t new_size = (n < 1000) ? 2000 : n + 1000;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0;
        
        // Zero out the new portion
        for (idx_t i = cache_size; i < new_size; i++) {
            new_cache[i] = 0;
        }
        
        cache = new_cache;
        cache_size = new_size;
    }
    
    if (cache[n] != 0) {
        return cache[n];
    }
    
    seq_t result;
    
    if (n == 1) {
        result = 2;
    } else if (n == 2) {
        result = 3;
    } else {
        // For n >= 3, find smallest product a(i)*a(j) > a(n-1)
        seq_t prev = a(n - 1);
        seq_t min_product = 0;
        bool found = false;
        
        for (idx_t i = 1; i < n - 1; i++) {
            seq_t ai = a(i);
            for (idx_t j = i + 1; j < n; j++) {
                seq_t aj = a(j);
                seq_t product = ai * aj;
                
                if (product > prev && (!found || product < min_product)) {
                    min_product = product;
                    found = true;
                }
            }
        }
        
        result = found ? min_product : 0;
    }
    
    cache[n] = result;
    return result;
}
