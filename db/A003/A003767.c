/*
 * A003767  Number of spanning trees in (K_4 - e) X P_n.
 * https://oeis.org/A003767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 140*a(n-1) - 1715*a(n-2) + 4952*a(n-3) - 1715*a(n-4) +
 *            140*a(n-5) - a(n-6), with a(1) = 8, a(2) = 1152, a(3) = 147000,
 *            a(4) = 18643968, a(5) = 2363741512, a(6) = 299675376000
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t base[] = {8, 1152, 147000, 18643968, 2363741512, 299675376000};
static seq_t *cache = NULL;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n >= 1 && n <= 6) {
        return base[n - 1];
    }
    
    if (cache == NULL) {
        idx_t initial_size = (n > 1000 ? n : 1000) + 100;
        cache = (seq_t *)malloc(initial_size * sizeof(seq_t));
        
        for (idx_t i = 0; i < 6; i++) {
            cache[i] = base[i];
        }
        
        max_computed = 6;
    } else if (n > max_computed) {
        idx_t new_size = n + 100;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i - 1] = (seq_t)140 * cache[i - 2]
                     - (seq_t)1715 * cache[i - 3]
                     + (seq_t)4952 * cache[i - 4]
                     - (seq_t)1715 * cache[i - 5]
                     + (seq_t)140 * cache[i - 6]
                     - cache[i - 7];
    }
    
    if (n > max_computed) {
        max_computed = n;
    }
    
    return cache[n - 1];
}
