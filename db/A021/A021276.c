/*
 * A021276  Decimal expansion of 1/272.
 * https://oeis.org/A021276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A021276: decimal digits of 1/272
   Recurrence for n > 12:
   a(n) = a(n-1) - a(n-8) + a(n-9)
   Initial values:
   a(0)=0, a(1)=0, a(2)=3, a(3)=6, a(4)=7, a(5)=6,
   a(6)=4, a(7)=7, a(8)=0, a(9)=5, a(10)=8, a(11)=8, a(12)=2
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 12;          /* highest index already stored */

    /* initial 13 terms */
    static const seq_t init[13] =
        {0, 0, 3, 6, 7, 6, 4, 7, 0, 5, 8, 8, 2};

    /* Direct lookup for the first 13 indices */
    if (n < 13) {
        return init[(size_t)n];
    }

    /* Allocate (or grow) the cache to accommodate index n */
    if (!cache) {                    /* first time we need more space */
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed – return 0 */

        /* initialise the cache with the first 13 terms */
        for (size_t i = 0; i < 13; ++i) {
            cache[i] = init[i];
        }
    } else if ((size_t)(n + 1) > cache_cap) { /* we need a bigger array */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;         /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Perform computation in a larger unsigned type to avoid underflow
               a(i) = a(i-1) - a(i-8) + a(i-9)
               = (a(i-1) + 10 + a(i-9) - a(i-8)) mod 10
               (adding 10 guarantees the intermediate is non‑negative) */
            unsigned long long tmp = (unsigned long long)cache[i - 1]
                                      + 10ULL
                                      + (unsigned long long)cache[i - 9]
                                      - (unsigned long long)cache[i - 8];
            cache[i] = (seq_t)(tmp % 10ULL);
        }
        max_computed = n;
    }

    return cache[n];
}
