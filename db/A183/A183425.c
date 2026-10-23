/*
 * A183425  Half the number of n X 2 0..2 arrays with no element equal its row
 *          sum plus its column sum mod 3.
 * https://oeis.org/A183425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n‑1) + a(n‑3) – 4*a(n‑4), with a(1)=3, a(2)=9,
 *            a(3)=32, a(4)=129
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent cache for previously computed values */
static seq_t *cache = NULL;          /* array of values, 1‑based index */
static size_t cache_cap = 0;         /* allocated capacity (>= n+1) */
static idx_t  computed_upto = 0;     /* largest n for which cache is fully known */

/* Ensure the cache is large enough and contains the seed values. */
static void ensure_cache(idx_t n)
{
    /* Guard against an insane n that cannot fit into size_t */
    if (n > SIZE_MAX - 1) return;  /* cannot allocate, leave cache unchanged */

    size_t needed = (size_t)n + 1;  /* we use indices 1..n inclusive, index 0 unused */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure – nothing else to do */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill the seed values if they have not yet been written */
    if (computed_upto < 1 && n >= 1) cache[1] = (seq_t)3;
    if (computed_upto < 2 && n >= 2) cache[2] = (seq_t)9;
    if (computed_upto < 3 && n >= 3) cache[3] = (seq_t)32;
    if (computed_upto < 4 && n >= 4) cache[4] = (seq_t)129;

    if (computed_upto < 4) {
        idx_t limit = n < 4 ? n : 4;
        computed_upto = limit;
    }
}

/* Compute a(n) for any n >= 1, memoising results in the static cache. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, normalise to 0 */

    /* Make sure the cache can hold at least n, and that the seeds are set. */
    ensure_cache(n);

    /* If we already know up to n, just return it. */
    if (computed_upto >= n) return cache[n];

    /* Compute values beyond the current limit up to n. */
    idx_t start = computed_upto + 1;
    if (start < 5) start = 5;      /* recurrence requires n >= 5 */

    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = 4*a(i-1) + a(i-3) - 4*a(i-4) */
        seq_t term = cache[i-1] * (seq_t)4;
        term += cache[i-3];
        term -= cache[i-4] * (seq_t)4;
        cache[i] = term;
    }

    computed_upto = n;
    return cache[n];
}
