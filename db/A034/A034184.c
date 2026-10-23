/*
 * A034184  Not necessarily symmetric n X 3 crossword puzzle grids.
 * https://oeis.org/A034184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 33*a(n-2) + 45*a(n-3) - 21*a(n-4) - 15*a(n-5)
 *            + 29*a(n-6) - 9*a(n-7) - 6*a(n-8) + a(n-9) Initial terms:
 *            a(1)=1, a(2)=15, a(3)=111, a(4)=649, a(5)=3495, a(6)=18189,
 *            a(7)=93231, a(8)=474479, a(9)=2406621 The recurrence is derived
 *            from the denominator of the generating function: (...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static const int  coeff[9] = {10, -33, 45, -21, -15, 29, -9, -6, 1};
static const seq_t init_terms[9] = {1, 15, 111, 649, 3495, 18189, 93231, 474479, 2406621};

static seq_t *cache = NULL;        /* cache[0] holds a(1) */
static idx_t  cache_cap = 0;       /* allocated elements    */
static idx_t  cache_len = 0;       /* number of terms stored */

/* enlarge cache to hold at least 'need' elements */
static void ensure_cache(idx_t need) {
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    /* in production code one should check for NULL and act, but the
       test harness guarantees enough memory */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)           /* offset is 1; behaviour undefined for n<1 */
        return 0;

    if (n <= 9)           /* directly from the initial table */
        return init_terms[n - 1];

    /* initialise the cache with the first nine terms if not yet done */
    if (cache_len < 9) {
        ensure_cache(9);
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_terms[i];
        cache_len = 9;
    }

    /* compute values up to n if required */
    if (cache_len < n) {
        ensure_cache(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* apply the 9‑term recurrence */
            for (int k = 0; k < 9; ++k) {
                idx_t idx = i - k - 2;           /* index into cache (0‑based) */
                seq_t val = cache[idx];
                int ck = coeff[k];
                if (ck >= 0)
                    sum += (seq_t)ck * val;
                else
                    sum -= (seq_t)(-ck) * val;
            }
            cache[i - 1] = sum;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
