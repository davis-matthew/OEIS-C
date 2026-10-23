/*
 * A169505  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the linear recurrence * derived from the
 *            rational generating function * * G(t) = (t^34 + 2*t^33 + … + 2*t
 *            + 1) / * (55*t^34 − 10*t^33 − … − 10*t + 1) * * a(0) = 1 * for n
 *            ≥ 1: * a(n) = 10·a(n−1) + 10·a(n−2) + … + 10·a(n−33) * −
 *            55·a(n−34) + p(n) * where p(n) = 1 if n = 0 or n = 3...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (highest index + 1) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Make sure the cache is large enough for index n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap <= (size_t)n) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;                  /* a(0) */
        } else {
            seq_t pos = 0;
            /* sum 10 * a(i - k) for k = 1 .. 34 where i >= k */
            for (idx_t k = 1; k <= 34; ++k) {
                if (i < k) break;      /* avoid underflow on i - k */
                pos += (seq_t)10 * cache[i - k];
            }
            /* subtract 55 * a(i-34) if available */
            if (i >= 34) {
                pos -= (seq_t)55 * cache[i - 34];
            }
            /* add p(i) term */
            if (i == 34) {
                pos += 1;
            } else {
                pos += 2;
            }
            val = pos;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
