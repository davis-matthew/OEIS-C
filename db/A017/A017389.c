/*
 * A017389  Expansion of g.f. 1/((1-3*x)*(1-5*x)*(1-7*x)).
 * https://oeis.org/A017389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 71*a(n-2) + 105*a(n-3), with a(0)=1, a(1)=15,
 *            a(2)=154
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;   /* not initialized */

    /* Initialize base cases on first call */
    if (last == (idx_t)-1) {
        grow_seq_array(&cache, &cap, (size_t)3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)154;
        last = 2;
    }

    /* Return if already computed */
    if (n <= last) return cache[n];

    /* Grow the array if necessary */
    if ((size_t)(n + 1) > cap)
        grow_seq_array(&cache, &cap, (size_t)(n + 1));

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)15 * cache[i - 1]
                 - (seq_t)71 * cache[i - 2]
                 + (seq_t)105 * cache[i - 3];
    }
    last = n;
    return cache[n];
}
