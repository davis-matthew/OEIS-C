/*
 * A042811  Denominators of continued fraction convergents to sqrt(936).
 * https://oeis.org/A042811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042811 – denominators of convergents to √936.
   Recurrence:
      a(n) = 10402*a(n-8) - a(n-16)   for n > 15
   Initial values (n = 0 … 15):
      1, 1, 2, 5, 32, 69, 101, 170,
      10301, 10471, 20772, 52015, 332862, 717739,
      1050601, 1768340
   The function memoises results and works for any order of calls. */
#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[16] = {
    1ULL, 1ULL, 2ULL, 5ULL, 32ULL, 69ULL,
    101ULL, 170ULL, 10301ULL, 10471ULL,
    20772ULL, 52015ULL, 332862ULL, 717739ULL,
    1050601ULL, 1768340ULL
};

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored */

    /* return cached value if already computed */
    if (n + 1 <= cache_size)
        return cache[n];

    /* grow cache to hold indices up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (tmp == NULL) /* allocation failure – undefined behaviour elsewhere */
        return 0;
    cache = tmp;

    /* compute missing values */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i < 16) {
            cache[i] = init_vals[(size_t)i];
        } else {
            cache[i] = ( (seq_t)10402 * cache[i - 8] ) - cache[i - 16];
        }
    }
    cache_size = new_size;
    return cache[n];
}
