/*
 * A097121  Expansion of (1-x)^2/((1-x)^3-4x^4).
 * https://oeis.org/A097121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 4*a(n-4), a(0) = a(1) =
 *            a(2) = a(3) = 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* this function is thread‑unsafe but works for single‑thread use
   required by the test harness */
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms          */
static idx_t  cache_len = 0;         /* number of terms stored           */
static idx_t  cache_cap = 0;         /* allocated capacity                */

/* grow the cache to hold at least (n+1) terms */
static void
grow_cache_to(idx_t n_plus_one)
{
    if (cache_cap < n_plus_one) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n_plus_one) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory: terminate */
        cache    = tmp;
        cache_cap = new_cap;
    }
}

static void
ensure_terms_up_to(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    grow_cache_to(n + 1);

    /* initialise the first four terms if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    /* compute remaining terms sequentially */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) + 4*a(i-4) */
        seq_t v1 = 3 * cache[i-1];
        seq_t v2 = 3 * cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = 4 * cache[i-4];
        cache[i] = v1 - v2 + v3 + v4;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* guarantee n is within bounds for the recursion */
    if (n < 4) {
        /* terms 0 to 3 are all equal to 1 */
        return (seq_t)1;
    }
    ensure_terms_up_to(n);
    return cache[n];
}
