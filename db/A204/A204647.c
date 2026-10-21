/*
 * A204647  Number of (n+1) X 5 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)-9*a(n-2)+5*a(n-3)+5*a(n-4)-9*a(n-5)+5*a(n-6)-a(n-7) for
 *            n>9 (initial terms: a(1)=48, a(2)=90, a(3)=178, a(4)=330,
 *            a(5)=571, a(6)=938, a(7)=1478, a(8)=2248, a(9)=3317)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoization cache: cache[i] holds a(i) */
static size_t cache_last = 0;   /* highest index computed, inclusive */

/* Return a(n) for any n ≥ 1, using memoization and the empirical recurrence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – n=0 never requested */

    /* initialise cache with the first nine terms if not already done */
    if (!cache) {
        cache_last = 9;
        cache = (seq_t *)malloc((cache_last + 1) * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed – unlikely for tests */

        cache[0] = 0;             /* unused – keeps indices 1‑based */
        cache[1] = 48;
        cache[2] = 90;
        cache[3] = 178;
        cache[4] = 330;
        cache[5] = 571;
        cache[6] = 938;
        cache[7] = 1478;
        cache[8] = 2248;
        cache[9] = 3317;
    }

    /* If the requested value is already cached, return it */
    if (n <= cache_last) return cache[n];

    /* Expand the cache to accommodate indices up to n */
    size_t new_len = (size_t)n + 1;          /* inclusive of n */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                      /* allocation failed */
    cache = tmp;

    /* Compute all new values using the recurrence */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        seq_t v = cache[i-1] * (seq_t)5
                 - cache[i-2] * (seq_t)9
                 + cache[i-3] * (seq_t)5
                 + cache[i-4] * (seq_t)5
                 - cache[i-5] * (seq_t)9
                 + cache[i-6] * (seq_t)5
                 - cache[i-7];
        cache[i] = v;
    }
    cache_last = (size_t)n;
    return cache[n];
}
