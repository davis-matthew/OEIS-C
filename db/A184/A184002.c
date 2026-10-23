/*
 * A184002  Number of (n+1) X 9 0..2 arrays with all 2 X 2 subblock sums the
 *          same.
 * https://oeis.org/A184002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 18, u64 n <= 39, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A184002 – Number of (n+1)×9 arrays with all 2×2 subblock sums equal
 * Recurrence (empirical, valid for n ≥ 10):
 *   a(n) = 6*a(n-1) - 60*a(n-3) + 85*a(n-4) + 150*a(n-5) - 360*a(n-6) 
 *          + 396*a(n-8) - 216*a(n-9)
 * Initial values a(1) … a(9) are hard‐coded.
 * The function memoises results in a static cache so that calls may arrive
 * in any order, each call runs in O(n) time where n is the requested
 * index.  Arbitrary‑precision arithmetic is supported via the seq_t type.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache 1‑based; seq_cache[0] unused */
static idx_t seq_cache_len = 0;   /* greatest index for which seq_cache is valid */

/* Ensure the cache can store values up to `n` (inclusive). */
static void ensure_cache(idx_t n)
{
    if (seq_cache_len >= n)
        return;
    size_t new_len = seq_cache_len > 0 ? seq_cache_len * 2 : 10;
    while (new_len < n)
        new_len *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * (new_len + 1));
    if (!tmp) abort();           /* allocation failure */
    seq_cache = tmp;
    seq_cache_len = new_len;
}

/* Initialise cache with the first nine terms. */
static void init_cache(void)
{
    if (seq_cache)
        return;                 /* already initialised */
    ensure_cache(9);
    seq_cache[0] = 0;           /* dummy */
    seq_cache[1] = (seq_t)35535;
    seq_cache[2] = (seq_t)40041;
    seq_cache[3] = (seq_t)47031;
    seq_cache[4] = (seq_t)61125;
    seq_cache[5] = (seq_t)85431;
    seq_cache[6] = (seq_t)134853;
    seq_cache[7] = (seq_t)227319;
    seq_cache[8] = (seq_t)418461;
    seq_cache[9] = (seq_t)799815;
    seq_cache_len = 9;
}

/* Computes a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n = 0, harmless */

    init_cache();

    if (n <= seq_cache_len)
        return seq_cache[n];

    /* Compute values sequentially up to n. */
    for (idx_t k = seq_cache_len + 1; k <= n; ++k) {
        ensure_cache(k);

        /* Calculate each term in full 64/128‑bit precision. */
        seq_t t1 = seq_cache[k-1] * (seq_t)6;
        seq_t t2 = seq_cache[k-3] * (seq_t)60;
        seq_t t3 = seq_cache[k-4] * (seq_t)85;
        seq_t t4 = seq_cache[k-5] * (seq_t)150;
        seq_t t5 = seq_cache[k-6] * (seq_t)360;
        seq_t t6 = seq_cache[k-8] * (seq_t)396;
        seq_t t7 = seq_cache[k-9] * (seq_t)216;

        seq_cache[k] = t1 - t2 + t3 + t4 - t5 + t6 - t7;
    }

    seq_cache_len = n; /* update cache length to avoid recomputation */
    return seq_cache[n];
}
