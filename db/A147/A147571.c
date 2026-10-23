/*
 * A147571  Numbers with exactly 4 distinct prime divisors {2,3,5,7}.
 * https://oeis.org/A147571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 210 * A002473(n) (product of 2,3,5,7 with exponents ≥1)
 * Range:     u8 n <= 1, u16 n <= 82, u32 n <= 2502, u64 n <= 8521,
 *            u128 n <= 8521, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();       /* out of memory */
        cache[0] = 1;              /* first element of A002473 */
        cache_size = 1;
    }

    /* extend cache until it contains at least n elements */
    while ((size_t)n > cache_size) {
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t c2 = cache[i2] * (seq_t)2;
        seq_t c3 = cache[i3] * (seq_t)3;
        seq_t c5 = cache[i5] * (seq_t)5;
        seq_t c7 = cache[i7] * (seq_t)7;

        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        cache[cache_size++] = next;

        if (next == c2) i2++;
        if (next == c3) i3++;
        if (next == c5) i5++;
        if (next == c7) i7++;
    }

    return cache[n-1] * (seq_t)210;
}
