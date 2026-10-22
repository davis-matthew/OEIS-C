/*
 * A006380  Number of equivalence classes of 4 X n binary matrices when one
 *          can permute rows, permute columns and complement columns.
 * https://oeis.org/A006380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (1 - x + x^2 + x^4 + x^6 - x^7 + x^8) / ((1 - x)^8*(1 +
 *            x)^2*(1 + x^2)*(1 + x + x^2)^2)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 122, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    // Coefficients of numerator P(x) = 1 - x + x^2 + x^4 + x^6 - x^7 + x^8
    static int p[] = {1, -1, 1, 0, 1, 0, 1, -1, 1};
    
    // Coefficients of denominator Q(x) = (1 - x)^8 * (1 + x)^2 * (1 + x^2) * (1 + x + x^2)^2
    static int q[] = {1, -4, 5, -2, 2, -2, -5, 8, -6, 8, -5, -2, 2, -2, 5, -4, 1};
    
    // Memoization
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 100;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
        cache[0] = 1;
    }
    
    // Expand cache if needed
    if (n >= cache_len) {
        idx_t new_len = n + 100;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        memset(cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache_len = new_len;
    }
    
    // Return cached value if available
    if (cache[n] != 0 || n == 0) {
        return cache[n];
    }
    
    // Compute using recurrence relation
    // a(n) = p_n - sum_{i=1}^{min(n, 16)} q[i] * a(n-i)
    // Split sum into positive and negative parts to handle unsigned arithmetic
    seq_t sum_pos = (seq_t)0, sum_neg = (seq_t)0;
    for (idx_t i = 1; i <= n && i < 17; i++) {
        seq_t a_i = a(n - i);
        if (q[i] > 0) {
            sum_pos = sum_pos + (seq_t)q[i] * a_i;
        } else if (q[i] < 0) {
            sum_neg = sum_neg + (seq_t)(-q[i]) * a_i;
        }
    }
    
    // Compute result: a(n) = p_n + sum_neg - sum_pos
    seq_t result;
    if (n < 9) {
        if (p[n] > 0) {
            result = (seq_t)p[n] + sum_neg - sum_pos;
        } else if (p[n] < 0) {
            result = sum_neg - sum_pos - (seq_t)(-p[n]);
        } else {
            result = sum_neg - sum_pos;
        }
    } else {
        result = sum_neg - sum_pos;
    }
    
    cache[n] = result;
    return result;
}
