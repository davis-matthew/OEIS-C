/*
 * A006673  E.g.f. is the logarithmic derivative of e.g.f. for Pell numbers
 *          [1, 0, 1, 2, 5, ...].
 * https://oeis.org/A006673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - Sum_{k=1..n-2} binomial(n-1,k) * a(k) *
 *            a(n-1-k), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    if (cache == NULL) {
        cache_len = n + 1;
        cache = mem_new(seq_t, cache_len);
        for (idx_t i = 0; i < cache_len; i++) {
            cache[i] = 0;
        }
        cache[1] = 1;
        
        for (idx_t i = 2; i <= n; i++) {
            seq_t sum = 0;
            for (idx_t k = 1; k < i - 1; k++) {
                seq_t binom_val = binomial(i - 1, k);
                sum = sum + binom_val * cache[k] * cache[i - 1 - k];
            }
            cache[i] = 2 * cache[i - 1] - sum;
        }
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = mem_resize(seq_t, cache, cache_len);
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = 0;
        }
        
        for (idx_t i = old_len; i <= n; i++) {
            seq_t sum = 0;
            for (idx_t k = 1; k < i - 1; k++) {
                seq_t binom_val = binomial(i - 1, k);
                sum = sum + binom_val * cache[k] * cache[i - 1 - k];
            }
            cache[i] = 2 * cache[i - 1] - sum;
        }
    }
    
    return cache[n];
}
