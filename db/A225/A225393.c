/*
 * A225393  Expansion of 1/(1 - x - x^2 + x^6 - x^8).
 * https://oeis.org/A225393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-6) + a(n-8) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=12, a(7)=19
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 49, u64 n <= 98, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;          /* computed terms */
static size_t cache_size = 0;          /* number of valid entries */

seq_t a(idx_t n)
{
    /* fast return if already cached */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* enlarge storage to hold up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    cache = tmp;

    /* compute missing terms */
    for (size_t i = cache_size; i < new_size; ++i) {
        if      (i == 0) cache[i] = 1;
        else if (i == 1) cache[i] = 1;
        else if (i == 2) cache[i] = 2;
        else if (i == 3) cache[i] = 3;
        else if (i == 4) cache[i] = 5;
        else if (i == 5) cache[i] = 8;
        else if (i == 6) cache[i] = 12;
        else if (i == 7) cache[i] = 19;
        else             cache[i] = cache[i-1] + cache[i-2] - cache[i-6] + cache[i-8];
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
