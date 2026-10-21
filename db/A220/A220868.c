/*
 * A220868  Number of perfect matchings in the twisted graph (C_4 X
 *          C_n)_{(13)(24)}.
 * https://oeis.org/A220868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)=3, a(2)=24, a(3)=54, a(4)=260, a(5)=726, a(6)=3096,
   a(7)=10086, a(8)=39940.
   For n>=9:
     a(n) = 3*a(n-1)+9*a(n-2)-19*a(n-3)-19*a(n-4)+9*a(n-5)+3*a(n-6)-a(n-7) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is 1-based, 0 is unused */
    if (n == 0)
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    size_t idx = (size_t)n;  /* n <= ~10000, safe for size_t */

    /* initialise cache with first 8 terms (index 0..8) */
    if (cache == NULL) {
        cache_cap = 9;                          /* indices 0..8 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            abort();                           /* allocation failure */

        cache[0] = 0;   /* not used, set to 0 for safety */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)24;
        cache[3] = (seq_t)54;
        cache[4] = (seq_t)260;
        cache[5] = (seq_t)726;
        cache[6] = (seq_t)3096;
        cache[7] = (seq_t)10086;
        cache[8] = (seq_t)39940;
    }

    /* extend cache if requested index is beyond current capacity */
    if (idx >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        /* compute missing terms using the 7‑term recurrence */
        for (size_t i = old_cap; i < new_cap; ++i) {
            __int128 v = 0;
            v += (__int128)cache[i-1] * 3;
            v += (__int128)cache[i-2] * 9;
            v -= (__int128)cache[i-3] * 19;
            v -= (__int128)cache[i-4] * 19;
            v += (__int128)cache[i-5] * 9;
            v += (__int128)cache[i-6] * 3;
            v -= (__int128)cache[i-7];
            cache[i] = (seq_t)v;
        }
    }

    return cache[idx];
}
