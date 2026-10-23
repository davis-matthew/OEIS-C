/*
 * A170125  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 37: a(n) = 18 * ∑_{k=1}^{n} a(n‑k) + 2 For
 *            n = 38: a(38) = 18 * ∑_{k=1}^{37} a(38‑k) – 171 a(0) + 1 For n >
 *            38: a(n) = 18 * ∑_{k=1}^{37} a(n‑k) – 171 a(n‑38)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------  Implementation  --------------------------------*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values               */
static size_t cache_cap = 0;         /* allocated capacity                    */
static size_t cache_len = 0;         /* number of valid entries in cache      */

/* ensure that cache has at least cap elements allocated */
static void ensure_cache_cap(size_t cap)
{
    if (cache_cap >= cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – terminate with error */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n), maintaining a static cache of computed terms */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    ensure_cache_cap(target + 1);

    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    for (size_t i = cache_len; i <= target; ++i) {
        seq_t sum = 0;
        /* sum of the previous at most 37 terms */
        for (int k = 1; k <= 37 && i >= (size_t)k; ++k) {
            sum += cache[i - k];
        }

        seq_t val = sum * (seq_t)18;

        if (i >= 38) {
            val -= (seq_t)171 * cache[i - 38];
        }

        if (i <= 38) {
            if (i == 0) {
                val = 1;                 /* already trivial but keep explicit */
            } else if (i == 38) {
                val += 1;                 /* numerator coefficient for a(38)   */
            } else {
                val += 2;                 /* numerator coefficient for 1≤i≤37 */
            }
        }

        cache[i] = val;
    }
    cache_len = target + 1;
    return cache[target];
}
