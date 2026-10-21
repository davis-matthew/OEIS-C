/*
 * A212365  Number of Dyck n-paths all of whose ascents and descents have
 *          lengths equal to 1 (mod 6).
 * https://oeis.org/A212365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k=1..n-6} a(k)*a(n-6-k) with a(0)=1 and
 *            a(1)=...=a(6)=1 (OEIS A212365)
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 53, u64 n <= 99, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization array */
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t  last  = 0;      /* highest index computed so far */

    /* Allocate the cache on first call or grow it if needed */
    if (cache == NULL) {
        /* start with room for at least 8 entries (covers small n) */
        cap   = (n > 8 ? (size_t)n + 1 : 8);
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */
        cache[0] = 1;                  /* a(0) = 1 */
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n, if not already done */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = 1; /* base cases a(1)..a(6) */
        } else {
            seq_t val = cache[i - 1];
            for (idx_t k = 1; k <= i - 6; ++k) {
                val += cache[k] * cache[i - 6 - k];
            }
            cache[i] = val;
        }
    }
    last = n;
    return cache[n];
}
