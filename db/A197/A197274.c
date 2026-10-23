/*
 * A197274  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,0,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A197274
   Recurrence (for n ≥ 8):
   a(n) = 2*a(n-1) + 2*a(n-3) - 2*a(n-4) - 2*a(n-5) - a(n-6) - 2*a(n-7)
   Initial values:
   a(1)=3, a(2)=9, a(3)=14, a(4)=29, a(5)=67, a(6)=134, a(7)=275
   Offset: 1 (a(1) is the first term)
*/

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

/* store initial values in a larger type to avoid compile‑time truncation */
static const unsigned long long _init_vals[8] = {
    0ULL,          /* a(0) (unused) */
    3ULL, 9ULL, 14ULL, 29ULL, 67ULL, 134ULL, 275ULL
};

seq_t a(idx_t n)
{
    /* cache of computed terms; static across calls */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* largest n computed so far */
    static size_t cache_cap = 0;   /* allocated slots (>= cached_max+1) */

    if (n == 0)          /* a(0) is undefined; return 0 */
        return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;                           /* indices 0..7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;             /* allocation failure */

        /* fill initial values (casting to seq_t at runtime) */
        cache[0] = (seq_t)_init_vals[0];
        cache[1] = (seq_t)_init_vals[1];
        cache[2] = (seq_t)_init_vals[2];
        cache[3] = (seq_t)_init_vals[3];
        cache[4] = (seq_t)_init_vals[4];
        cache[5] = (seq_t)_init_vals[5];
        cache[6] = (seq_t)_init_vals[6];
        cache[7] = (seq_t)_init_vals[7];
        cached_max = 7;
    }

    /* grow cache if needed */
    if (n > cached_max) {
        size_t needed_cap = (size_t)n + 1;          /* indices 0..n inclusive */
        if (needed_cap > cache_cap) {
            seq_t *new_cache = (seq_t *)malloc(needed_cap * sizeof(seq_t));
            if (new_cache == NULL) return 0;       /* allocation failure */

            /* copy existing values */
            for (idx_t i = 0; i <= cached_max; ++i)
                new_cache[i] = cache[i];

            free(cache);
            cache     = new_cache;
            cache_cap = needed_cap;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* recurrence uses a(i-1)...a(i-7); i ≥ 8 here */
            seq_t term = 0;

            term += cache[i-1];
            term += cache[i-1];          /* 2*a(i-1) */

            term += cache[i-3];
            term += cache[i-3];          /* 2*a(i-3) */

            term -= cache[i-4];
            term -= cache[i-4];          /* -2*a(i-4) */

            term -= cache[i-5];
            term -= cache[i-5];          /* -2*a(i-5) */

            term -= cache[i-6];          /* -a(i-6) */

            term -= cache[i-7];
            term -= cache[i-7];          /* -2*a(i-7) */

            cache[i] = term;
        }
        cached_max = n;
    }

    return cache[(size_t)n];
}
