/*
 * A025897  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A025897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6)+a(n-7)+a(n-8)-a(n-13)-a(n-14)-a(n-15)+a(n-21)
 *            Initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=a(5)=0
 * Range:     u8 n <= 401, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size */
    static idx_t  computed_up_to = 0;  /* highest index that has been computed */
    static bool   initialized   = false;

    /* Initialise the cache on first call. */
    if (!initialized) {
        initialized   = true;
        cache_size    = 6;                      /* 0 .. 5 */
        cache         = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;                    /* should never happen in tests */
        cache[0]      = 1;                       /* a(0) */
        computed_up_to= 0;                       /* last computed index */
    }

    /* Extend the cache if we need to reach 'n'. */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure */
        cache     = tmp;
        /* Initialise the newly allocated entries to 0. */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute terms up to n if they are not already available. */
    if (n > computed_up_to) {
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            long long val = 0;

            if (k >= 6)  val += (long long)cache[k - 6];
            if (k >= 7)  val += (long long)cache[k - 7];
            if (k >= 8)  val += (long long)cache[k - 8];
            if (k >= 13) val -= (long long)cache[k - 13];
            if (k >= 14) val -= (long long)cache[k - 14];
            if (k >= 15) val -= (long long)cache[k - 15];
            if (k >= 21) val += (long long)cache[k - 21];

            cache[k] = (seq_t)val;   /* final result is always non‑negative */
        }
        computed_up_to = n;
    }

    return cache[n];
}
