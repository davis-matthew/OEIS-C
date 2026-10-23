/*
 * A170427  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For 1 <= n < 44: a(n) = 34 * 33^(n-1) * For n >=
 *            44: a(n) = 32 * (a(n-1)+...+a(n-43)) - 528 * a(n-44)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_cache, big_pow, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Utility functions supplied by the build */
#include "../utils/big_pow.h"
#include "../utils/alloc_cache.h"
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* actual allocated size */
static idx_t last_computed = 0;   /* largest n for which cache[n] is valid */
static seq_t sum43 = 0;           /* used only when n >= 44 */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Allocate storage if necessary */
    if (cache == NULL) {
        /* Ensure we have room for at least 44 terms (the base cases) */
        size_t init_cap = (n >= 44) ? (size_t)(n + 1) : 44;
        cache = alloc_cache(init_cap);
        cache_cap = init_cap;
        cache[0] = 1;            /* a(0) = 1 */
        last_computed = 0;
    }

    /* Grow the cache when a larger n is requested */
    if ((size_t)(n + 1) > cache_cap) {
        grow_seq_array(&cache, &cache_cap, n + 1);
    }

    /* Fill missing entries up to n */
    if (n > last_computed) {
        idx_t start = last_computed + 1;
        /* ----------------------------------------------------------------
           Constants used in the recurrence */
        const seq_t C_34  = (seq_t)34;
        const seq_t C_33  = (seq_t)33;
        const seq_t C_32  = (seq_t)32;
        const seq_t C_528 = (seq_t)528;
        /* ---------------------------------------------------------------- */

        /* Prepare sum of the last 43 terms when we hit the first recurrence
           at n = 44. */
        if (start <= 44) {
            idx_t limit = (n < 44) ? n : 44;
            for (idx_t i = start; i <= limit; ++i) {
                if (i == 0) {
                    cache[0] = 1;
                } else if (i < 44) {
                    /* a(i) = 34 * 33^(i-1) */
                    cache[i] = C_34 * big_pow(C_33, (seq_t)(i - 1));
                } else { /* i == 44 */
                    /* compute sum of a(1)..a(43) */
                    sum43 = 0;
                    for (idx_t j = 1; j <= 43; ++j) {
                        sum43 += cache[j];
                    }
                }
            }
            start = limit + 1;
        }

        /* From now on we are in the recurrence region (n >= 44)         */
        for (idx_t i = start; i <= n; ++i) {
            /* Recurrence: a(i) = 32 * sum_{k=1}^{43} a(i-k) - 528 * a(i-44) */
            cache[i] = C_32 * sum43 - C_528 * cache[i - 44];
            /* Update the window sum for the next iteration */
            sum43 += cache[i] - cache[i - 43];
        }

        last_computed = n;
    }

    return cache[n];
}
