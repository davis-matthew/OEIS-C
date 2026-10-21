/*
 * A224561  Number of (n+3) X 4 0..1 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 2*a(n-5) +
 *            2*a(n-6) - 2*a(n-7) + a(n-8), for n > 10 Initial values:
 *            a(1)=452, a(2)=370, a(3)=512, a(4)=739, a(5)=990, a(6)=1345,
 *            a(7)=1852, a(8)=2659, a(9)=3846, a(10)=5589
 * Range:     u8 none, u16 n <= 16, u32 n <= 45, u64 n <= 103, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[10] = {
    (seq_t)452ULL, (seq_t)370ULL, (seq_t)512ULL, (seq_t)739ULL,
    (seq_t)990ULL, (seq_t)1345ULL, (seq_t)1852ULL, (seq_t)2659ULL,
    (seq_t)3846ULL, (seq_t)5589ULL
};

static seq_t *cache = NULL;     /* memoization array, 1‑based */
static size_t cache_cap = 0;    /* allocated capacity in elements */
static idx_t cache_len = 0;     /* highest index that has been computed */

/* ensure capacity for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;

    size_t new_cap = (cache_cap == 0 ? (size_t)(n + 1) : cache_cap);
    while (new_cap <= (size_t)(n + 1)) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard – offset starts at 1 */

    if (n <= 10) return init_vals[n - 1];

    /* compute terms up to n, using memoisation */
    if (n > cache_len) {
        ensure_cache(n);

        /* initialise the first ten terms if they are not yet stored */
        if (cache_len < 10) {
            for (idx_t i = cache_len + 1; i <= 10; ++i)
                cache[i] = init_vals[i - 1];
            cache_len = 10;
        }

        /* apply the recurrence for the remaining indices */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val = 3 * cache[i - 1]
                      - 3 * cache[i - 2]
                      + cache[i - 3]
                      + cache[i - 4]
                      - 2 * cache[i - 5]
                      + 2 * cache[i - 6]
                      - 2 * cache[i - 7]
                      + cache[i - 8];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
