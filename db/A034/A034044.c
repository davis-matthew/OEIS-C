/*
 * A034044  Numbers that are primitively but not imprimitively represented by
 *          x^2+y^2+z^2.
 * https://oeis.org/A034044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that is a sum of three squares,
 *            primitively represented by x² + y² + z², and not divisible by
 *            any square > 1 whose quotient is also a sum of three squares
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   ensure_capacity, is_sum_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility for dynamic capacity management, provided by the build system */
#include "../utils/ensure_capacity.h"
#include "../utils/is_sum_three_squares.h"

/* Main sequence function.  The result is returned in type seq_t,
   and the index is given in type idx_t. */
seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next = 1;          /* Next integer to test (1‑based) */

    /* Undefined for n = 0: return 0 as a safe placeholder. */
    if (n == 0) {
        return 0;
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t m = (seq_t)next;   /* Candidate integer */
        ++next;

        /* Check if m itself is sum of three squares. */
        if (!is_sum_three_squares(m)) {
            continue;
        }

        /* Primitive test:
           For every square divisor d² (d > 1) of m, ensure that m/d²
           is NOT a sum of three squares. */
        bool primitive = true;
        for (idx_t d = 2; (seq_t)d * (seq_t)d <= m; ++d) {
            seq_t sq = (seq_t)d * (seq_t)d;
            if (m % sq == 0) {
                seq_t q = m / sq;
                if (is_sum_three_squares(q)) {
                    primitive = false;
                    break;
                }
            }
        }

        if (primitive) {
            /* Grow cache if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];
}
