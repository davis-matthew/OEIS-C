/*
 * A021179  Decimal expansion of 1/175.
 * https://oeis.org/A021179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) Initial terms: a(0)=0, a(1)=0,
 *            a(2)=5, a(3)=7, a(4)=1, a(5)=4 This recurrence generates the
 *            decimal digits of 1/175
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static size_t cap   = 0;      /* allocated capacity */
static size_t len   = 0;      /* number of computed terms */

/* initialize cache with base terms */
static void init_cache(void)
{
    if (cache == NULL) {
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 5;
        cache[3] = 7;
        cache[4] = 1;
        cache[5] = 4;
        len = 6;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    init_cache();

    if (n >= len) {
        /* grow capacity if necessary */
        if ((size_t)(n + 1) > cap) {
            size_t newcap = cap * 2;
            if (newcap < (size_t)(n + 1))
                newcap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) {
                /* allocation failed; for correctness let's refuse */
                abort();
            }
            cache = tmp;
            cap = newcap;
        }

        /* compute missing terms */
        for (idx_t i = len; i <= n; ++i) {
            seq_t val = cache[i - 1] - cache[i - 3] + cache[i - 4];
            cache[i] = val;
        }
        len = n + 1;
    }

    return cache[n];
}
