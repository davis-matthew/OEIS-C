/*
 * A047247  Numbers that are congruent to {2, 3, 4, 5} (mod 6).
 * https://oeis.org/A047247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n > 5 with initial values
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=5, a(5)=8
 * Range:     u8 n <= 170, u16 n <= 9675, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  The array indices are
       zero‑based, so cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently cached */
    static size_t cache_cap = 0;   /* allocated capacity (in terms) */

    if (n == 0)                     /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    size_t idx = (size_t)n;          /* index in 0‑based array */

    /* --- Grow the cache if needed ------------------------------------- */
    if (idx > cache_len) {
        size_t required = idx;

        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 8;
            while (new_cap < required)
                new_cap <<= 1;        /* double until enough space */

            /* realloc returns void*, cast to the correct pointer type.
               This is valid in both C and C++ (compatible with C++17).   */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms.  i runs from cache_len+1 up to idx
           (1‑based indices). */
        for (size_t i = cache_len + 1; i <= idx; ++i) {
            if (i <= 5) {      /* initial values */
                switch (i) {
                case 1:  cache[i - 1] = (seq_t)2; break;
                case 2:  cache[i - 1] = (seq_t)3; break;
                case 3:  cache[i - 1] = (seq_t)4; break;
                case 4:  cache[i - 1] = (seq_t)5; break;
                case 5:  cache[i - 1] = (seq_t)8; break;
                }
            } else {           /* recurrence: a(i) = a(i-1) + a(i-4) - a(i-5) */
                cache[i - 1] = cache[i - 2] + cache[i - 5] - cache[i - 6];
            }
        }
        cache_len = idx;
    }

    return cache[idx - 1];
}
