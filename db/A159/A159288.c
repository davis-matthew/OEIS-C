/*
 * A159288  Expansion of (1 + x + x^2)/(1 - x^2 - 2*x^3).
 * https://oeis.org/A159288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) for n > 2 with initial values a(0) = 1,
 *            a(1) = 1, a(2) = 2
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 106,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)2;
            } else {
                cache[i] = cache[i - 2] + (seq_t)2 * cache[i - 3];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
