/*
 * A120775  a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3).
 * https://oeis.org/A120775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3); a(1)=1, a(2)=6, a(3)=23
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation cache */
static idx_t cache_size = 0;  /* number of values stored in cache */

/*
 * Computes the nth term of OEIS A120775.
 * The function is memoised; calls may come in any order.
 */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n == 0 is irrelevant, return 0 */
    if (n == 0)
        return (seq_t)0;

    /* If we already have the value, return it immediately */
    if (n <= cache_size)
        return cache[n - 1];

    /* Extend the cache to at least n terms */
    idx_t old_size = cache_size;
    idx_t new_size = (old_size == 0) ? 3 : old_size * 2;
    if (new_size < n)
        new_size = n;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failure – no recovery */
    cache = tmp;

    /* Initialise the first three terms if this is the first allocation */
    if (old_size == 0) {
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)6;          /* a(2) */
        cache[2] = (seq_t)23;         /* a(3) */
        old_size = 3;
    }

    /* Compute missing terms using the recurrence */
    idx_t start = old_size;
    if (start < 3)
        start = 3;

    for (idx_t i = start; i < new_size; ++i) {
        cache[i] = (seq_t)((seq_t)3 * cache[i - 1] +
                           (seq_t)5 * cache[i - 2] +
                           cache[i - 3]);
    }

    cache_size = new_size;
    return cache[n - 1];
}
