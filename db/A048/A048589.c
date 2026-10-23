/*
 * A048589  Pisot sequence L(7,9).
 * https://oeis.org/A048589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) a(0) = 7, a(1) = 9,
 *            a(2) = 12, a(3) = 16, a(4) = 22
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamically grown array of terms */
static size_t cache_size = 0; /* number of terms currently stored */

static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_size = 5; /* indices 0 … 4 are the first 5 terms */
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    cache[0] = (seq_t)7;
    cache[1] = (seq_t)9;
    cache[2] = (seq_t)12;
    cache[3] = (seq_t)16;
    cache[4] = (seq_t)22;
}

seq_t a(idx_t n)
{
    init_cache();

    /* Already known */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the array to hold up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0; /* allocation failure – unlikely in tests */
    cache = tmp;

    /* Compute missing terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t v = cache[i - 1];
        v += v;                 /* 2*a(i-1) */
        v -= cache[i - 2];      /* -a(i-2) */
        v += cache[i - 3];      /* +a(i-3) */
        v -= cache[i - 4];      /* -a(i-4) */
        cache[i] = v;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
