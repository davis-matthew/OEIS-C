/*
 * A155069  Expansion of (3 - x - sqrt(1 - 6*x + x^2))/2.
 * https://oeis.org/A155069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 recurrence for n >= 1: a(n) = sum_{k=1}^{n-1} a(k) *
 *            a(n-k) + a(n-1) This follows from the generating function G(x) =
 *            (3 - x - sqrt(1 - 6*x + x^2)) / 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 1265 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* allocated capacity */
    static idx_t last = 0;     /* highest index already computed */

    /* initialise cache on first call */
    if (!cache) {
        grow_seq_array(&cache, &cap, 1);
        cache[0] = (seq_t)1;
    }

    /* extend cache and compute any missing values up to n */
    if (n > last) {
        grow_seq_array(&cache, &cap, (size_t)(n + 1));
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* sum_{k=1}^{i-1} a(k) * a(i-k) */
            for (idx_t k = 1; k < i; ++k) {
                sum += cache[k] * cache[i - k];
            }
            /* add a(i-1) */
            sum += cache[i - 1];
            cache[i] = sum;
        }
        last = n;
    }

    return cache[n];
}
