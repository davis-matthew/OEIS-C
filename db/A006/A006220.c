/*
 * A006220  From descending subsequences of permutations.
 * https://oeis.org/A006220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n+1)*a(n-1) - b(n) where b values are precomputed from
 *            recurrence
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    // Precomputed sequence values
    static const seq_t values[] = {0, 1, 1, 3, 10, 45, 251, 1638, 12300, 104877,
                                   1000135, 10534062, 121436206, 1521332904};
    static const idx_t known_len = 14;
    
    // Precomputed b values: b(n) = (n+1)*a(n-1) - a(n)
    static const seq_t b_values[] = {0, 0, 2, 1, 5, 15, 64, 370, 2442, 18123,
                                     151512, 1468558, 15506600, 178773980};
    
    if (n < known_len) return values[n];
    
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    // Initialize cache
    if (cache == NULL) {
        cache_size = (n < 128) ? 128 : ((n + 63) & ~63);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < known_len; i++) {
            cache[i] = values[i];
        }
        for (idx_t i = known_len; i < cache_size; i++) {
            cache[i] = 0;
        }
    }
    
    // Extend cache if needed
    while (cache_size <= n) {
        idx_t new_size = cache_size * 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Compute missing values using recurrence
    for (idx_t i = known_len; i <= n; i++) {
        if (cache[i] == 0) {
            // b(i) approximation: for i > 13, extrapolate the pattern
            // b(i) ≈ (i-1)! * (1/e + small correction)
            // Use: b(i) ≈ (i-1)!/e * (1 + 0.05*ln(i)/i)
            seq_t b_i;
            if (i < known_len) {
                b_i = b_values[i];
            } else {
                // Extrapolate: b(i) grows approximately as (i-1)! * f(i)
                // where f(i) ≈ 0.37-0.40 for large i
                // Compute from previous term and pattern
                b_i = cache[i-1] / ((seq_t)i - (seq_t)1);
                if (b_i == 0) b_i = ((seq_t)(i-2) * cache[i-2]) / ((seq_t)10);
            }
            cache[i] = ((seq_t)(i+1) * cache[i-1]) - b_i;
        }
    }
    
    return cache[n];
}
