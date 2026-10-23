/*
 * A145646  Wavelength (in ångströms) of the series limit of the Hydrogen
 *          spectrum for main quantum number n.
 * https://oeis.org/A145646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5) for
 *            n>5 with initial values a(1)=911, a(2)=3645, a(3)=8201,
 *            a(4)=14580, a(5)=22782
 * Range:     u8 none, u16 n <= 8, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[1] .. cache[cache_len] */
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise cache with first 5 terms */
    if (!cache) {
        size_t init_cap = 6;        /* indices 0..5  (index 0 unused) */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed */
        cache[1] = (seq_t)911;
        cache[2] = (seq_t)3645;
        cache[3] = (seq_t)8201;
        cache[4] = (seq_t)14580;
        cache[5] = (seq_t)22782;
        cache_len = 5;
    }

    if (n <= cache_len) return cache[n];

    /* extend cache to length n (indices 1..n) */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;           /* reallocation failed */
    cache = tmp;

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t a_n =
            (seq_t)3 * cache[i-1]
          - (seq_t)4 * cache[i-2]
          + (seq_t)4 * cache[i-3]
          - (seq_t)3 * cache[i-4]
          +               cache[i-5];
        cache[i] = a_n;
    }

    cache_len = n;
    return cache[n];
}
