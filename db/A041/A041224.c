/*
 * A041224  Numerators of continued fraction convergents to sqrt(124).
 * https://oeis.org/A041224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   period_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 11,
 *   a(1) = 78,
 *   for n ≥ 2:
 *     a(n) = coeff(n) * a(n-1) + a(n-2),
 *   where coeff(n) is periodic with period 16:
 *     coeff(1)  = 7,  coeff(2)  = 2,  coeff(3)  = 1,  coeff(4)  = 1,
 *     coeff(5)  = 1,  coeff(6)  = 3,  coeff(7)  = 1,  coeff(8)  = 4,
 *     coeff(9)  = 1,  coeff(10) = 3,  coeff(11) = 1,  coeff(12) = 1,
 *     coeff(13) = 1,  coeff(14) = 2,  coeff(15) = 7,  coeff(16) = 22.
 *   For n > 16, coeff(n) repeats starting again at 7.
 *
 * These coefficients come from the periodic continued‑fraction expansion
 * of √124.  The sequence a(n) thus gives the numerators of the convergents
 * to √124.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/period_index.h"

/* Utility: multiply two seq_t values with natural unsigned overflow. */
static inline seq_t mul_seq(seq_t x, seq_t y) { return x * y; }

static const seq_t period[16] = {
    (seq_t)7, (seq_t)2, (seq_t)1, (seq_t)1,
    (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)4,
    (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)1,
    (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)22
};

static seq_t *cache = NULL;     /* cached numerators */
static size_t  cap   = 0;        /* allocated capacity (number of slots) */
static size_t  size  = 0;        /* number of numerators computed */

/* Ensure the cache is large enough to hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (size <= n) {
        /* Grow the buffer if necessary. */
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = (cap == 0 ? 16 : cap * 2);
            while (new_cap < needed) new_cap *= 2;
            seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_arr;
            cap = new_cap;
        }

        /* Initialize the first terms if they are not yet set. */
        if (size == 0) {
            cache[0] = (seq_t)11;
            size = 1;
        }
        if (size == 1 && n >= 1) {
            cache[1] = (seq_t)78;
            size = 2;
        }

        /* Compute any missing terms up to index n. */
        for (size_t i = size; i <= (size_t)n; ++i) {
            seq_t c = period[period_index(i)];
            cache[i] = mul_seq(c, cache[i - 1]) + cache[i - 2];
        }
        size = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
