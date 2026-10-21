/*
 * A207365  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 45, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207365
   Linear recurrence (order 16):
   a(n) =  2*a(n-1) + 6*a(n-2) -14*a(n-3) -14*a(n-4) +42*a(n-5)
          +14*a(n-6) -70*a(n-7)        +70*a(n-9) -14*a(n-10)
          -42*a(n-11)+14*a(n-12)+14*a(n-13)-6*a(n-14)-2*a(n-15)+a(n-16)
   Initial values (a(1) … a(16)):
   13, 169, 624, 2304, 5856, 14884, 31110, 65025,
   120105, 221841, 375858, 636804, 1011864, 1607824,
   2430756, 3674889
*/

#include <stdlib.h>

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    /* The cache is 1‑based; cache[0] is unused. */
    static seq_t *cache = NULL;          /* pointer to allocated array */
    static idx_t  cache_size = 0;        /* allocated size minus 1 (max index) */
    static idx_t  max_computed = 0;      /* highest index that has a valid value */

    /* ------------------------------------------------------------------ */
    /* Initialise the cache with the first 16 values.                       */
    /* ------------------------------------------------------------------ */
    static const seq_t init_vals[17] = {
        0,          /* dummy for index 0 */
        13, 169, 624, 2304, 5856, 14884, 31110, 65025,
        120105, 221841, 375858, 636804, 1011864, 1607824,
        2430756, 3674889
    };
    static const int coeff[16] = {
        2,   6, -14, -14,  42,  14, -70,   0,
       70, -14, -42,  14,  14,  -6,  -2,   1
    };

    /* ------------------------------------------------------------------ */
    /* Allocate or enlarge the cache if necessary.                         */
    /* ------------------------------------------------------------------ */
    if (cache_size == 0 || cache_size < 16) {
        cache = (seq_t*)malloc((16 + 1) * sizeof(seq_t));
        cache_size = 16;
    }
    if (cache_size < n) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_size = n;
    }

    /* ------------------------------------------------------------------ */
    /* Set the first 16 entries (once, or after a reallocation).          */
    /* ------------------------------------------------------------------ */
    if (max_computed < 16) {
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init_vals[i];
        max_computed = 16;
    }

    /* ------------------------------------------------------------------ */
    /* If the requested term is already computed, just return it.         */
    /* ------------------------------------------------------------------ */
    if (n <= max_computed)
        return cache[n];

    /* ------------------------------------------------------------------ */
    /* Compute missing terms using the linear recurrence.                 */
    /* ------------------------------------------------------------------ */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int j = 0; j < 16; ++j) {
            int c = coeff[j];
            if (c > 0)
                sum += (seq_t)c * cache[k - j - 1];
            else if (c < 0)
                sum -= (seq_t)(-c) * cache[k - j - 1];
        }
        cache[k] = sum;
    }
    max_computed = n;
    return cache[n];
}
