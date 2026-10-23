/*
 * A049343  Numbers m such that 2m and m^2 have same digit sum.
 * https://oeis.org/A049343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   a(n) is the nth non‑negative integer m (≥0) such that
 *            digit_sum(2*m) == digit_sum(m*m). Indexing starts at n = 1. The
 *            sequence records these m in increasing order
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Forward declaration of utils provided by the framework. */
seq_t digit_sum(seq_t v);

seq_t a(idx_t n) {
    /* Static cache of discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_checked_m = 0;   /* m the next integer to test */

    /* Ensure we have at least n terms. */
    while (cache_size < (size_t)n) {
        seq_t m = (seq_t)last_checked_m;
        seq_t two_m = m * (seq_t)2;
        seq_t m_sq = m * m;

        if (digit_sum(two_m) == digit_sum(m_sq)) {
            /* Grow cache if necessary. */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* realloc can return NULL only if allocation fails; in that
                   case, the program will terminate as per the build’s
                   expectations. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = m;
        }
        ++last_checked_m;
    }

    /* n is 1‑based. */
    return cache[n-1];
}
