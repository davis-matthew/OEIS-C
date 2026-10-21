/*
 * A062039  a(0) = 1; a(n+1) = a(n) - n if a(n) > n, otherwise a(n+1) = a(n) +
 *          a(a(n)).
 * https://oeis.org/A062039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   For n ≥ 0:
      if a(n) > n
          a(n+1) = a(n) – n
      else
          a(n+1) = a(n) + a(a(n))   */
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_len = 0;      /* number of initialised elements */
static idx_t cache_cap = 0;      /* allocated capacity */

/* Grow the cache so that indices up to `need` can be stored. */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;                     /* double until enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Initialise cache on the first call. */
static void init_cache(void)
{
    if (cache == NULL) {
        ensure_capacity(0);                 /* allocate space for a(0) */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        cache_len = 1;
    }
}

static seq_t get_value(idx_t idx) { return cache[idx]; }
static void set_value(idx_t idx, seq_t v) { cache[idx] = v; }

seq_t a(idx_t n)
{
    init_cache();                    /* lazy initialisation */

    if (n < cache_len)              /* already computed */
        return get_value(n);

    /* Ensure we have enough storage for all indices up to n. */
    ensure_capacity(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t prev_idx = i - 1;              /* index of a(i-1) */
        seq_t prev = get_value(prev_idx);    /* a(i-1) */

        seq_t cur;
        if (prev > (seq_t)prev_idx) {
            /* a(i) = a(i-1) - (i-1) */
            cur = prev - (seq_t)prev_idx;
        } else {
            /* a(i) = a(i-1) + a(a(i-1)) */
            idx_t inner = (idx_t)prev;      /* prev ≤ i-1, so inner < cache_len */
            cur = prev + get_value(inner);
        }
        set_value(i, cur);
    }
    cache_len = n + 1;               /* update number of initialised elements */
    return get_value(n);
}
