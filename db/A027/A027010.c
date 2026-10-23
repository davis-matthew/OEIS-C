/*
 * A027010  a(n) = Sum_{k=floor((n+1)/2)..n} T(k,n-k); i.e., a(n) is the n-th
 *          diagonal sum of left-justified array T given by A026998.
 * https://oeis.org/A027010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 55, u64 n <= 110,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:   a(1)=1, a(2)=1, a(3)=2, a(4)=5, a(5)=6
 *               a(n)=a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)+a(n-5)   for n≥6
 * Uses memoised recursion to handle arbitrary call order. */
#include <stdlib.h>

static seq_t *cache = NULL;  /* 1‑based cache of computed terms */
static idx_t  cache_size = 0;/* highest index stored in the cache */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for offset   */

    /* Initialise cache with the first five terms. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)(5 + 1));
        if (!cache) exit(1);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)6;
        cache_size = 5;
    }

    /* Grow cache if the requested index is beyond the current size. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                      sizeof(seq_t) * (size_t)(n + 1));
        if (!tmp) exit(1);
        cache = tmp;
        idx_t old_size = cache_size;

        /* Compute missing terms up to n using the recurrence. */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            /* sumFirst = a(i-1) + a(i-5) + 2*a(i-2) */
            seq_t sum = cache[i-1] + cache[i-5];
            sum += cache[i-2]; /* + a(i-2) */
            sum += cache[i-2]; /* + a(i-2) again */

            /* subFirst = a(i-3) + 2*a(i-4) */
            seq_t sub = cache[i-3];
            sub += cache[i-4];
            sub += cache[i-4];

            cache[i] = sum - sub; /* ensures no unsigned underflow */
        }
        cache_size = n;
    }

    return cache[n];
}
