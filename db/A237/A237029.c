/*
 * A237029  Number of (n+1) X (2+1) 0..2 arrays with the maximum of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A237029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) +31*a(n-2) -1351*a(n-3) +2166*a(n-4)
 *            +3313*a(n-5) -5940*a(n-6) +1760*a(n-7) (empirical recurrence,
 *            initial values a(1)=563, a(2)=11205, a(3)=221994, a(4)=4552617,
 *            a(5)=93224611, a(6)=1918085364, a(7)=39451935691)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_last = 0;

    /* Valid indices start at 1 */
    if (n < 1) return (seq_t)0;

    /* Initialise cache on first call */
    if (cache_cap == 0) {
        /* We need at least 8 entries: indices 0..7 (0 unused) */
        cache_cap = (n + 1 > 8) ? (n + 1) : 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failure */
        /* Initial values */
        static const seq_t init[8] = {
            (seq_t)0ULL,          /* a(0) unused */
            (seq_t)563ULL,
            (seq_t)11205ULL,
            (seq_t)221994ULL,
            (seq_t)4552617ULL,
            (seq_t)93224611ULL,
            (seq_t)1918085364ULL,
            (seq_t)39451935691ULL
        };
        for (idx_t i = 1; i < cache_cap && i <= 7; ++i)
            cache[i] = init[i];
        cache_last = (cache_cap > 7) ? 7 : (cache_cap - 1);
    }

    /* Grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
        /* Fill new slots if we didn't have initial values yet */
        for (idx_t i = cache_last + 1; i <= 7 && i < cache_cap; ++i) {
            static const seq_t init[8] = {
                (seq_t)0ULL,
                (seq_t)563ULL,
                (seq_t)11205ULL,
                (seq_t)221994ULL,
                (seq_t)4552617ULL,
                (seq_t)93224611ULL,
                (seq_t)1918085364ULL,
                (seq_t)39451935691ULL
            };
            cache[i] = init[i];
        }
    }

    /* If we need nothing beyond what we already have, just return */
    if (n <= cache_last) return cache[n];

    /* Compute terms from cache_last+1 up to n using the recurrence */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        /* All coefficients are non‑negative except the subtractions.  We
           compute the sum of positive terms first and subtract the
           negative contribution.  The result is guaranteed non‑negative
           for this recurrence.  */
        seq_t pos = cache[k - 1] * (seq_t)22ULL
                  + cache[k - 2] * (seq_t)31ULL
                  + cache[k - 4] * (seq_t)2166ULL
                  + cache[k - 5] * (seq_t)3313ULL
                  + cache[k - 7] * (seq_t)1760ULL;
        seq_t neg = cache[k - 3] * (seq_t)1351ULL
                  + cache[k - 6] * (seq_t)5940ULL;
        cache[k] = pos - neg;
    }
    cache_last = n;
    return cache[n];
}
