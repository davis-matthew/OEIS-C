/*
 * A061090  Squares the sum of the squares of whose digits are squares.
 * https://oeis.org/A061090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This sequence is defined by searching successive perfect
 *            squares. The n‑th term is the n‑th square whose sum of the
 *            squares of its decimal digits is itself a perfect square. No
 *            closed formula is known, so the function builds the sequence
 *            incrementally
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 2423, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/floor_sqrt.h"

/* Return the n‑th term (1‑based) of OEIS A061090. */
seq_t a(idx_t n)
{
    /* static state for incremental, order‑independent construction */
    static seq_t *cache      = NULL;   /* cached terms */
    static size_t cache_cap = 0;       /* current capacity */
    static size_t cache_len = 0;       /* number of terms stored */
    static idx_t  checked_m = 0;       /* largest m processed (m^2 has been tested) */

    /* initial allocation of the cache */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache     = seq_alloc(cache_cap);
    }

    /* produce terms until we have at least n of them */
    while (cache_len < n) {
        ++checked_m;
        /* square of m */
        seq_t sq = seq_mul((seq_t)checked_m, (seq_t)checked_m);

        /* sum of squares of the decimal digits of sq */
        seq_t temp = sq;
        uint64_t digit_sq_sum = 0;
        while (temp > 0) {
            uint64_t d = (uint64_t)(temp % 10);
            digit_sq_sum += d * d;
            temp /= 10;
        }

        /* check whether this sum is a perfect square */
        seq_t root = floor_sqrt((seq_t)digit_sq_sum);
        if (seq_mul(root, root) == (seq_t)digit_sq_sum) {
            /* store term in the cache, expanding if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
    }

    return cache[n - 1];
}
