/*
 * A156567  Positive numbers y such that y^2 is of the form x^2+(x+23)^2 with
 *          integer x.
 * https://oeis.org/A156567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6 Initial values (n = 1 … 6):
 *            a(1)=17, a(2)=23, a(3)=37, a(4)=65, a(5)=115, a(6)=205 (All
 *            other a(n) are positive integers.)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc for C, malloc for C++ */
#include <string.h>   /* memcpy for C, also available in C++ */

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1; return 0 for invalid */
        return 0;

    /* Static cache: elements 0 … cache_len-1 store a(1) … a(cache_len) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached terms */

    /* Grow cache if needed */
    if (n > cache_len) {
        /* Allocate new array of required size */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * n);
        if (new_cache == NULL)
            return 0;   /* allocation failed */

        /* Copy old data if any, then free old array */
        if (cache != NULL) {
            memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
            free(cache);
        }

        /* Zero out newly allocated part (values will be computed later) */
        for (idx_t i = cache_len; i < n; ++i)
            new_cache[i] = 0;

        cache = new_cache;
        idx_t old_len = cache_len;
        cache_len = n;

        /* Compute all missing values from old_len to n-1 */
        for (idx_t i = old_len; i < n; ++i) {
            seq_t val;
            if (i < 6) {           /* base cases (indices 0..5 represent n=1..6) */
                switch (i) {
                    case 0: val = 17; break;
                    case 1: val = 23; break;
                    case 2: val = 37; break;
                    case 3: val = 65; break;
                    case 4: val = 115; break;
                    case 5: val = 205; break;
                    default: val = 0;  /* should never occur */
                }
            } else { /* recurrence for n > 6 */
                val = 6 * cache[i - 3] - cache[i - 6];
            }
            cache[i] = val;
        }
    } else {
        /* Ensure all entries up to n-1 are present (may happen if calls
           are non‑sequential). Compute any missing ones. */
        for (idx_t i = 0; i < n; ++i) {
            if (cache[i] != 0) continue;  /* already computed */
            if (i < 6) {
                switch (i) {
                    case 0: cache[i] = 17; break;
                    case 1: cache[i] = 23; break;
                    case 2: cache[i] = 37; break;
                    case 3: cache[i] = 65; break;
                    case 4: cache[i] = 115; break;
                    case 5: cache[i] = 205; break;
                }
            } else {
                cache[i] = 6 * cache[i - 3] - cache[i - 6];
            }
        }
    }

    return cache[n - 1];
}
