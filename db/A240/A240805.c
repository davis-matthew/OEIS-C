/*
 * A240805  Coefficients in expansion of graph zeta function for complete
 *          graph K_4.
 * https://oeis.org/A240805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            8*a(n-3)+6*a(n-4)-16*a(n-6)-24*a(n-7)+3*a(n-8)+16*a(n-9)+24*a(n-10)-16*a(n-12)
 *            for n>11 Initial terms: a(0)=1, a(1)=0, a(2)=0, a(3)=8, a(4)=6,
 *            a(5)=0, a(6)=48, a(7)=72, a(8)=39, a(9)=272, a(10)=600,
 *            a(11)=624
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {
    1, 0, 0, 8, 6, 0, 48, 72, 39, 272, 600, 624
};

static seq_t *cache = NULL;
static size_t cache_sz = 0;

static void ensure_cache(idx_t n)
{
    /* Ensure the cache can hold indices up to n. */
    size_t need = (size_t)n + 1;
    if (cache_sz >= need) return;

    seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    cache = tmp;

    for (size_t i = cache_sz; i < need; ++i) {
        if (i < 12) {
            cache[i] = init[i];
        } else {
            seq_t v =
                8 * cache[i-3] +
                6 * cache[i-4] -
               16 * cache[i-6] -
               24 * cache[i-7] +
                3 * cache[i-8] +
               16 * cache[i-9] +
               24 * cache[i-10] -
               16 * cache[i-12];
            cache[i] = v;
        }
    }
    cache_sz = need;
}

seq_t a(idx_t n)
{
    if (n < 12) return init[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
