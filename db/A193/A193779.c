/*
 * A193779  Expansion of f(-x) * f(-x^15) / (f(-x^3) * f(-x^5)) in powers of x
 *          where f() is a Ramanujan theta function.
 * https://oeis.org/A193779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of f(-x)*f(-x^15)/(f(-x^3)*f(-x^5)) The generating
 *            function equals the infinite product Π_{n≥1, n mod
 *            15∈{1,2,4,7,8,11,13,14}} (1 - x^n). The coefficient of x^n is
 *            produced by multiplying out the factors (1 - x^k) for all
 *            admissible k
 * Range:     i8 n <= 74, i16 n <= 203, i32 n <= 638, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cached coefficients           */
static idx_t a_cache_len = 0;   /* number of cached entries      */

seq_t a(idx_t n)
{
    if (n >= a_cache_len) { /* grow cache to include n           */
        size_t needed = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!new_cache) exit(1);                    /* allocation failure */

        /* initialise */
        new_cache[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            new_cache[i] = 0;

        /* multiply by (1 - x^k) for each admissible k   */
        for (idx_t k = 1; k <= n; ++k) {
            /* skip k if it is a multiple of 3 or 5      */
            if (k % 3 == 0 || k % 5 == 0)
                continue;
            /* apply the factor to all coefficients ≥ k */
            for (idx_t i = n; i >= k; --i)
                new_cache[i] -= new_cache[i - k];
        }

        free(a_cache);
        a_cache = new_cache;
        a_cache_len = n + 1;
    }

    return a_cache[(size_t)n];
}
