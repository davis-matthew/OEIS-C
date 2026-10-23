/*
 * A193643  Number of arrays of -3..3 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + a(n-3) with a(1)=1, a(2)=7,
 *            a(3)=19. Indices start at 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* for realloc, abort */

static seq_t *a_cache = NULL;   /* 1‑based indexed cache */
static idx_t a_cache_len = 0;   /* highest index that is valid in a_cache */

/* enlarge the cache to hold indices up to n (inclusive) */
static void a_grow_cache(idx_t n)
{
    if (n <= a_cache_len) return;            /* already large enough */

    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(a_cache, (new_len + 1) * sizeof(seq_t));
    if (!new_cache) abort();                 /* memory allocation failure */

    a_cache = new_cache;
    /* initialise newly added entries to zero */
    for (idx_t i = a_cache_len + 1; i <= new_len; ++i) a_cache[i] = 0;
    a_cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for out‑of‑range calls */

    a_grow_cache(n);

    /* initialise base values if they become accessible */
    if (a_cache[1] == 0 && n >= 1) a_cache[1] = 1;
    if (a_cache[2] == 0 && n >= 2) a_cache[2] = 7;
    if (a_cache[3] == 0 && n >= 3) a_cache[3] = 19;

    if (n <= 3) return a_cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = 4; i <= n; ++i)
    {
        if (a_cache[i] == 0)
            a_cache[i] = 2 * a_cache[i-1]
                         + 4 * a_cache[i-2]
                         + a_cache[i-3];
    }

    return a_cache[n];
}
