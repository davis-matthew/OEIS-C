/*
 * A041547  Denominators of continued fraction convergents to sqrt(291).
 * https://oeis.org/A041547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 580*a(n-2) - a(n-4); a(0)=1, a(1)=17, a(2)=579, a(3)=9860
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n < cache_size)
        return cache[n];

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)17;
        else if (i == 2)
            cache[i] = (seq_t)579;
        else if (i == 3)
            cache[i] = (seq_t)9860;
        else
            cache[i] = ((seq_t)580) * cache[i - 2] - cache[i - 4];
    }

    cache_size = new_size;
    return cache[n];
}
