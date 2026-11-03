/*
 * A123274  a(0)=a(1)=a(2) = 1. a(n) = (a(n-1) +a(n-2))
 *          /GCD(a(n-1)+a(n-2),a(n-3)), for n >= 3.
 * https://oeis.org/A123274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-2))/gcd(a(n-1)+a(n-2), a(n-3)), with
 *            a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* External linkage function */
seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array holding computed terms */
    static idx_t  computed = 0;      /* highest index computed + 1 */
    static size_t capacity = 0;      /* allocated size of cache array */

    /* Grow the cache array if needed */
    if ((size_t)(n + 1) > capacity) {
        grow_seq(&cache, &capacity, (size_t)(n + 1));
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = cache[i-1] + cache[i-2];
            seq_t g   = gcd(sum, cache[i-3]);
            cache[i]  = sum / g;
        }
    }
    /* Update computed count to reflect new range */
    computed = n + 1;

    return cache[n];
}
