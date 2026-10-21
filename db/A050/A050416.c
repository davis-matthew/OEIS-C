/*
 * A050416  a(1)=a(2)=1, then a(n+1) = floor(a(n)/3) if this is not among 0,
 *          a(1), ..., a(n); otherwise a(n+1) = a(n) + a(n-1).
 * https://oeis.org/A050416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2051, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=1; for n≥2,
   a(n+1)=floor(a(n)/3) if floor(a(n)/3)∉{0,a(1),…,a(n));
   otherwise a(n+1)=a(n)+a(n-1). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache of already computed terms.
       cache[i] holds a(i) for 1 ≤ i ≤ cached_n. */
    static seq_t *cache = NULL;
    static size_t cached_n = 0;

    if (n == 0) return 0; /* undefined for n<1, but harmless */

    size_t target = (size_t)n;
    if (target <= cached_n)               /* already computed */
        return cache[target];

    /* enlarge cache to accommodate 'target' */
    size_t new_cap = target + 1; /* need indices 0..target */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* memory exhaustion */
    cache = new_cache;

    /* initialise first two terms if they were not already set */
    if (cached_n == 0) {
        cache[1] = 1;
        if (new_cap > 2)
            cache[2] = 1;
    } else if (cached_n == 1) {
        cache[2] = 1;
    }

    /* compute terms from cached_n+1 up to target */
    for (size_t k = cached_n + 1; k <= target; ++k) {
        if (k <= 2) { /* safety, though handled above */
            cache[k] = 1;
            continue;
        }
        seq_t prev = cache[k - 1];
        seq_t cand = prev / 3;         /* floor division, unsigned */

        bool used = (cand == 0);
        if (!used) {
            for (size_t i = 1; i < k; ++i) {
                if (cache[i] == cand) {
                    used = true;
                    break;
                }
            }
        }

        if (used)
            cache[k] = prev + cache[k - 2];
        else
            cache[k] = cand;
    }

    cached_n = target;
    return cache[target];
}
