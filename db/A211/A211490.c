/*
 * A211490  Number of (n+1) X (n+1) -4..4 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or two distinct values.
 * https://oeis.org/A211490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) (for n ≥ 4), a(1)=13, a(2)=17, a(3)=23
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence in a static array.  It is
   safe for any seq_t type that can be constructed by default and
   assigns to with copies of the type.  For arbitrary‑precision types
   that require construction, the build system supplies a suitable
   seq_t that is compatible with this usage. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values, indexed from 1 */
static idx_t cached_len = 0;  /* highest index currently stored */

/* Return the n‑th term of the sequence, with a(1) = 13, a(2) = 17,
   and a(3) = 23.  Behaviour for n < 1 is undefined, we return 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset is 1 */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..3 (index 0 unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (!cache) return 0;                /* allocation failure */

        cache[0] = 0;                       /* unused */
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)23;
        cached_len = 3;
    }

    /* if the desired term is already computed, return it */
    if (n <= cached_len)
        return cache[n];

    /* grow the cache to hold the new index */
    size_t needed = (size_t)(n + 1);         /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
    if (!tmp) return 0;                     /* allocation failure */
    cache = tmp;

    /* compute any missing terms using the recurrence */
    for (idx_t i = cached_len + 1; i <= n; ++i)
        cache[i] = cache[i - 1] * (seq_t)2 - cache[i - 3];

    cached_len = n;
    return cache[n];
}
