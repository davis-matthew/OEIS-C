/*
 * A221930  Number of 2Xn arrays of occupancy after each element stays put or
 *          moves to some horizontal, diagonal or antidiagonal neighbor,
 *          without move-in move-out straight through or left turns.
 * https://oeis.org/A221930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence for A221930:
   a(n) =  9*a(n-1) +10*a(n-2) -45*a(n-3) -57*a(n-4) +45*a(n-5)
          -3*a(n-6) -616*a(n-7) -57*a(n-8) +1200*a(n-9) -268*a(n-10)
          -289*a(n-11) -9*a(n-12) +6*a(n-13)
   Initial values (n = 1 … 13):
   1, 25, 232, 2197, 20933, 198589, 1885324,
   17896442, 169874679, 1612469540, 15305706000,
   145283086944, 1379039583394 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* offset begins at 1 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* allocated capacity (in terms) */
    static size_t cache_size = 0;   /* number of terms currently stored */

    /* initialise cache for the first 13 terms on first call */
    if (cache == NULL) {
        cache_cap = 13;
        cache_size = 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* memory‑allocation failed */

        cache[0] = 1;
        cache[1] = 25;
        cache[2] = 232;
        cache[3] = 2197;
        cache[4] = 20933;
        cache[5] = 198589;
        cache[6] = 1885324;
        cache[7] = 17896442;
        cache[8] = 169874679;
        cache[9] = 1612469540;
        cache[10] = 15305706000;
        cache[11] = 145283086944;
        cache[12] = 1379039583394;
    }

    /* grow cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 13;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* memory‑allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_size; i < (size_t)n; ++i) {      /* i is 0‑based */
        if (i < 13) {              /* already initialised */
            /* nothing to do */
        } else {
            cache[i] =
                9   * cache[i-1]  + 10 * cache[i-2] - 45 * cache[i-3]
              - 57  * cache[i-4] + 45 * cache[i-5]  - 3  * cache[i-6]
              - 616 * cache[i-7] - 57 * cache[i-8]  + 1200 * cache[i-9]
              - 268 * cache[i-10] - 289 * cache[i-11] - 9 * cache[i-12]
              + 6   * cache[i-13];
        }
    }
    cache_size = (size_t)n;

    return cache[(size_t)n - 1];
}
