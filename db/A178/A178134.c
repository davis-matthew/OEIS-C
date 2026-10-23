/*
 * A178134  Sum_{m=0..(n-1)/2} A176263(n-m-1, m).
 * https://oeis.org/A178134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 7*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) -
 *            25*a(n-6) + 5*a(n-7) + 25*a(n-8) for n >= 8; initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=-3, a(5)=-2, a(6)=-32,
 *            a(7)=-81
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 23, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---- base values for n = 0..7 ------------------------------------ */
    static const seq_t base[8] = {0, 1, 1, 2, -3, -2, -32, -81};

    /* ---- cache of computed terms ------------------------------------ */
    static seq_t *cache = NULL;   /* array a(0)..a(m) */
    static size_t cache_len = 0;  /* number of stored values = last_index+1 */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* ---- return base value directly if n < 8 ----------------------- */
    if (n < 8)
        return base[n];

    /* ---- initialise cache if first call ---------------------------- */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = base[i];
        cache_len = 8;
    }

    /* ---- grow cache if necessary ----------------------------------- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1; /* double capacity */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                        /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---- compute missing terms up to n ---------------------------- */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val =
            cache[i - 1]
          + (seq_t)7   * cache[i - 2]
          - (seq_t)2   * cache[i - 3]
          - (seq_t)6   * cache[i - 4]
          - (seq_t)4   * cache[i - 5]
          - (seq_t)25  * cache[i - 6]
          + (seq_t)5   * cache[i - 7]
          + (seq_t)25  * cache[i - 8];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
