/*
 * A099212  a(n) = -2*a(n-1) + 4*a(n-3), with a(0) = 1, a(1) = a(2) = 0.
 * https://oeis.org/A099212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 4*a(n-3), a(0)=1, a(1)=0, a(2)=0
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 35, i64 n <= 71, i128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N < cache_len)
        return cache[N];

    size_t newlen = N + 1;
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!tmp)
        abort(); /* memory allocation failed */

    cache = tmp;
    for (size_t i = cache_len; i < newlen; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1 || i == 2)
            val = (seq_t)0;
        else
            val = -((seq_t)2 * cache[i - 1]) + ((seq_t)4 * cache[i - 3]);

        cache[i] = val;
    }

    cache_len = newlen;
    return cache[N];
}
