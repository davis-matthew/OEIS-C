/*
 * A224575  Number of (n+5) X 11 0..1 matrices with each 6 X 6 subblock
 *          idempotent.
 * https://oeis.org/A224575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) + a(n-3) + 2*a(n-6) -5*a(n-7)
 *            +4*a(n-8) - a(n-9) - a(n-12) +2*a(n-13) - a(n-14) for n > 18.
 *            Initial values (n = 1..18): a(1)=25769, a(2)=7386, a(3)=7884,
 *            a(4)=8714, a(5)=9480, a(6)=10086, a(7)=10561, a(8)=12373,
 *            a(9)=15413, a(10)=19404, a(11)=24072, a(12)=29230, a(13)=34...
 * Range:     u8 none, u16 n <= 15, u32 n <= 60, u64 n <= 148, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[18] = {
    (seq_t)25769, (seq_t)7386, (seq_t)7884, (seq_t)8714,
    (seq_t)9480, (seq_t)10086, (seq_t)10561, (seq_t)12373,
    (seq_t)15413, (seq_t)19404, (seq_t)24072, (seq_t)29230,
    (seq_t)34734, (seq_t)41919, (seq_t)52000, (seq_t)65945,
    (seq_t)84434, (seq_t)107959
};

static seq_t *cache = NULL;      /* cache[i] = a(i) for i >= 1 */
static size_t cache_cap = 0;     /* allocated size (index 0 unused) */
static size_t cache_len = 0;     /* highest index currently populated */

/* Helper to allocate or grow the cache to accommodate indices up to `max_idx`. */
static void ensure_cache(size_t max_idx)
{
    if (max_idx >= cache_cap) {
        size_t new_cap = max_idx + 1;   /* +1 because we ignore index 0 */
        if (new_cap < 19) new_cap = 19; /* reserve space for initial 18 values */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Initialize any new entries with zero (for safety, though they will
       be overwritten by the recurrence before use). */
    for (size_t i = cache_len + 1; i <= cache_cap - 1; ++i)
        cache[i] = 0;
}

/* Static helper to copy the known initial terms into the cache if not already done. */
static void maybe_initialise_cache(void)
{
    if (cache_len >= 18) return; /* already done */
    for (size_t i = 1; i <= 18 && i < cache_cap; ++i)
        cache[i] = init[i - 1];
    if (cache_len < 18) cache_len = 18;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range; not used */
    size_t idx = (size_t)n;

    ensure_cache(idx);
    maybe_initialise_cache();

    if (idx <= cache_len) return cache[idx];

    /* Compute missing values up to idx (starting from 19). */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        if (i <= 18) continue;          /* already set by init */
        /* Recurrence for i > 18 */
        cache[i] =
            (seq_t)3 * cache[i - 1]
            - (seq_t)3 * cache[i - 2]
            + cache[i - 3]
            + (seq_t)2 * cache[i - 6]
            - (seq_t)5 * cache[i - 7]
            + (seq_t)4 * cache[i - 8]
            - cache[i - 9]
            - cache[i - 12]
            + (seq_t)2 * cache[i - 13]
            - cache[i - 14];
    }

    cache_len = idx;
    return cache[idx];
}
