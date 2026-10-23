/*
 * A159285  Expansion of (1+3*x)/(1-x^2-2*x^3).
 * https://oeis.org/A159285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 52, u64 n <= 105,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=3, a(2)=1, a(3)=5, a(n)=a(n-2)+2*a(n-3) (OEIS) */
#include <stdlib.h>
#include <stddef.h>
static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n < cache_cap) return cache[(size_t)n];
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* memory allocation failure */
    cache = tmp;
    for (size_t i = cache_cap; i < new_cap; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)3;
        } else if (i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)5;
        } else {
            cache[i] = cache[i - 2] + ( (seq_t)2 * cache[i - 3] );
        }
    }
    cache_cap = new_cap;
    return cache[(size_t)n];
}
