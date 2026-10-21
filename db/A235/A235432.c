/*
 * A235432  Number of (n+1) X (1+1) 0..2 arrays with the difference between
 *          each 2 X 2 subblock maximum and minimum lexicographically
 *          nondecreasing rowwise and columnwise.
 * https://oeis.org/A235432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - a(n-2) - 64*a(n-3) - 14*a(n-4) + 88*a(n-5) +
 *            8*a(n-6) - 24*a(n-7) with initial terms: a(1)=81, a(2)=583,
 *            a(3)=3987, a(4)=26091, a(5)=167095, a(6)=1054515, a(7)=6595119
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation; indices start at 1 */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;
static idx_t  CACHE_MAX = 0;

/* initial values (indices 1..7) */
static const seq_t BASE[8] = {81, 583, 3987, 26091, 167095, 1054515, 6595119};

seq_t a(idx_t n)
{
    /* return directly if within the initial segment */
    if (n <= 7U) return BASE[n - 1U];

    /* expand cache to accommodate index n */
    if (n > CACHE_MAX) {
        size_t needed = (size_t)n + 1U;   /* indices 0..n, we ignore 0 */

        /* first allocation */
        if (CACHE == NULL) {
            CACHE = (seq_t *)malloc(needed * sizeof(seq_t));
            if (!CACHE) return 0;          /* out‑of‑memory, return 0 as sentinel */
            CACHE_CAP = needed;
            /* zero out storage (especially index 0 which is unused) */
            for (size_t i = 0; i < needed; ++i) CACHE[i] = 0;
            /* initialise known terms */
            for (idx_t i = 1U; i <= 7U; ++i) CACHE[i] = BASE[i - 1U];
            CACHE_MAX = 7U;
        }
        /* grow the allocation if necessary */
        else if (needed > CACHE_CAP) {
            seq_t *tmp = (seq_t *)realloc(CACHE, needed * sizeof(seq_t));
            if (!tmp) return 0;              /* allocation failed */
            CACHE = tmp;
            /* zero new space */
            for (size_t i = CACHE_CAP; i < needed; ++i) CACHE[i] = 0;
            CACHE_CAP = needed;
        }

        /* compute missing terms up to n */
        for (idx_t i = CACHE_MAX + 1U; i <= n; ++i) {
            seq_t val = (seq_t)8ULL * CACHE[i - 1U];
            val -= CACHE[i - 2U];
            val -= (seq_t)64ULL * CACHE[i - 3U];
            val -= (seq_t)14ULL * CACHE[i - 4U];
            val += (seq_t)88ULL * CACHE[i - 5U];
            val += (seq_t)8ULL * CACHE[i - 6U];
            val -= (seq_t)24ULL * CACHE[i - 7U];
            CACHE[i] = val;
        }
        CACHE_MAX = n;
    }

    return CACHE[n];
}
