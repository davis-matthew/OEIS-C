/*
 * A063345  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          72 ).
 * https://oeis.org/A063345
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n‑2) + a(n‑4) – a(n‑6), for n > 8 Initial values: a(2)
 *            = -1, a(3) = 55, a(4) = 119, a(5) = 184, a(6) = 247, a(7) = 312,
 *            a(8) = 375
 * Range:     i8 n <= 4, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i >= 0.
       computed up to cached_max (inclusive). */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;
    static size_t cache_cap = 0;

    /* Ensure the cache exists. */
    if (cache == NULL) {
        cache_cap = 64;               /* arbitrary initial size */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;         /* allocation failed – unlikely */

        /* Set initial values up to n = 8. */
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)55;
        cache[4] = (seq_t)119;
        cache[5] = (seq_t)184;
        cache[6] = (seq_t)247;
        cache[7] = (seq_t)312;
        cache[8] = (seq_t)375;
        cached_max = 8;
    }

    /* Expand cache if necessary. */
    if (n > cached_max) {
        if (n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failed */
            /* zero the newly allocated part */
            for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n. */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 8) {
                /* Initial values by table. */
                switch ((int)i) {
                    case 2: cache[i] = (seq_t)-1; break;
                    case 3: cache[i] = (seq_t)55; break;
                    case 4: cache[i] = (seq_t)119; break;
                    case 5: cache[i] = (seq_t)184; break;
                    case 6: cache[i] = (seq_t)247; break;
                    case 7: cache[i] = (seq_t)312; break;
                    case 8: cache[i] = (seq_t)375; break;
                }
            } else {
                /* Recurrence: a(i) = a(i-2) + a(i-4) - a(i-6). */
                cache[i] = cache[i - 2] + cache[i - 4] - cache[i - 6];
            }
        }
        cached_max = n;
    }

    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2) return 0;

    return cache[n];
}
