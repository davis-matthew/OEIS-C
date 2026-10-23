/*
 * A152046  a(n) = Product_{k=1..floor((n-1)/2)} (1 + 8*cos(k*Pi/n)^2) for n
 *          >= 0.
 * https://oeis.org/A152046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) for n>2, a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 2;   /* index of the last computed value */

/* Return the n‑th term of OEIS sequence A152046 */
seq_t a(idx_t n)
{
    /* base values */
    if (n <= 2)
        return 1;

    /* lazy allocation and initialization of the cache */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1) < 4 ? 4 : (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                     /* out of memory */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache_last = 2;
    }

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                     /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill in the missing terms using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + (cache[i - 2] << 1);   /* a(i)=a(i-1)+2*a(i-2) */
    }
    cache_last = n;

    return cache[n];
}
