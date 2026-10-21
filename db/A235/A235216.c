/*
 * A235216  Number of (n+1) X (1+1) 0..2 arrays with 2 X 2 edge jumps all no
 *          more than +1 in one of the clockwise or counterclockwise
 *          directions or both.
 * https://oeis.org/A235216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 9*a(n-2) - 6*a(n-3) - 2*a(n-4); a(1)=59,
 *            a(2)=427, a(3)=3037, a(4)=21693
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence was found empirically; coefficients are as given in OEIS. */

#include <stdlib.h>

static seq_t *cache = NULL;        /* storage for computed terms */
static size_t cache_cap   = 0;      /* allocated capacity of cache        */
static size_t cache_len   = 0;      /* number of terms actually stored    */

/* Ensure the cache can hold at least 'need' terms (1‑based). */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    if (cache_cap == 0) cache_cap = 4;                 /* start with small capacity */
    while (cache_cap < need) cache_cap <<= 1;          /* grow by factor 2 */
    seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);                /* unrecoverable error */
    cache = new_cache;
}

/* Compute a(n) for n >= 1, using recurrence or memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                      /* undefined below offset */

    size_t limit = (size_t)n;                         /* 1‑based count */

    /* Grow cache if needed */
    ensure_cache_capacity(limit);

    /* Compute any missing values */
    for (size_t j = cache_len; j < limit; ++j) {      /* j is 0‑based index */
        seq_t value;
        switch (j) {                                 /* j 0->a(1), 1->a(2) ... */
            case 0: value = (seq_t)59; break;
            case 1: value = (seq_t)427; break;
            case 2: value = (seq_t)3037; break;
            case 3: value = (seq_t)21693; break;
            default:                                 /* recurrence for j>=4 */
                seq_t a1 = cache[j - 1];              /* a(j)  = a(j-1)+1 */
                seq_t a2 = cache[j - 2];
                seq_t a3 = cache[j - 3];
                seq_t a4 = cache[j - 4];
                value = a1 * 6 + a2 * 9 - a3 * 6 - a4 * 2;
        }
        cache[j] = value;
    }

    cache_len = limit;                                /* mark all up to 'n' computed */
    return cache[n - 1];
}
