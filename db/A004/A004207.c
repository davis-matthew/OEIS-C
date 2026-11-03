/*
 * A004207  a(0) = 1, a(n) = sum of digits of all previous terms.
 * https://oeis.org/A004207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1) + sum_of_digits(a(n-1)) for n
 *            >= 2
 * Range:     u8 n <= 29, u16 n <= 3395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9996 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n >= cache_len) {
        cache = mem_resize(seq_t, cache, (n + 1));
        
        for (idx_t i = cache_len; i <= n; i++) {
            if (i <= 1) {
                cache[i] = 1;
            } else {
                cache[i] = cache[i - 1] + digit_sum(cache[i - 1]);
            }
        }
        
        cache_len = n + 1;
    }
    
    return cache[n];
}
