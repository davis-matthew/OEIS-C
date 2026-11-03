/*
 * A061383  Arithmetic mean of digits is an integer.
 * https://oeis.org/A061383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth integer x for which digit_sum(x) % digit_len(x)
 *            == 0
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9913 (2026-11-03)
 * Helpers:   digit_sum, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Helper: tests whether the arithmetic mean of the decimal digits of x is an integer */
static inline bool mean_is_integer(seq_t x) {
    seq_t sum = digit_sum(x);
    int len = num_digits(x);
    return (len > 0) && ((sum % (seq_t)len) == 0);
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cached sequence values   */
    static size_t cap   = 0;      /* current capacity of cache*/
    static idx_t size   = 0;      /* number of values stored   */
    static idx_t cur    = 0;      /* next integer to test      */

    /* Return already computed term if available */
    if (n < size) return cache[n];

    /* Generate terms until we have a(n) */
    while (size <= n) {
        if (mean_is_integer(cur)) {
            if (size >= cap)           /* grow cache if needed   */
                ensure_cache(size, &cache, &cap);
            cache[size++] = cur;       /* store the new term      */
        }
        ++cur;                         /* advance to next integer */
    }
    return cache[n];
}
