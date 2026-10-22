/*
 * A003633  The sequence 2^(1-n)*a(n) is fixed (up to signs) by Stirling2
 *          transform.
 * https://oeis.org/A003633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(n) = -sum_{k=1}^{n-1} a(k) * S(n, k) * 2^(n-k-1), a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 11, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;
    
    // Initialize cache on first use
    if (cache == NULL) {
        cache_size = 1024;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;
        cache[1] = 1;
        cache_computed = 1;
    }
    
    // Extend cache if necessary
    while (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Return if already computed
    if (n <= cache_computed) return cache[n];
    
    // Compute up to n
    for (idx_t m = cache_computed + 1; m <= n; m++) {
        // Stirling numbers of second kind S(i, k)
        seq_t *s_prev = (seq_t *)calloc(m + 1, sizeof(seq_t));
        seq_t *s_curr = (seq_t *)calloc(m + 1, sizeof(seq_t));
        if (!s_prev || !s_curr) {
            free(s_prev);
            free(s_curr);
            return 0;
        }
        
        s_prev[1] = 1;  // S(1, 1) = 1
        
        for (idx_t row = 2; row <= m; row++) {
            s_curr[1] = 1;
            for (idx_t col = 2; col < row; col++) {
                s_curr[col] = (seq_t)col * s_prev[col] + s_prev[col - 1];
            }
            s_curr[row] = 1;
            
            seq_t *tmp = s_prev;
            s_prev = s_curr;
            s_curr = tmp;
        }
        
        // Compute a(m) = -sum_{k=1}^{m-1} a(k) * S(m, k) * 2^(m-k-1)
        seq_t result = 0;
        for (idx_t k = 1; k < m; k++) {
            seq_t ak = cache[k];
            seq_t snk = s_prev[k];
            seq_t power2 = (seq_t)1 << (m - k - 1);
            result -= ak * snk * power2;
        }
        
        cache[m] = result;
        free(s_prev);
        free(s_curr);
    }
    
    cache_computed = n;
    return cache[n];
}
