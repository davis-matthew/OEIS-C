/*
 * A166415  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A166415
   a(n) = 19*Sum_{j=1..10} a(n-j) - 190*a(n-11)   for n >= 12
   a(11) = 19*Sum_{j=1..10} a(11-j) - 190*a(0) + 1
   a(0) = 1; a(1) = 21; a(2) = 420; a(3) = 8400;
   a(4) = 168000; a(5) = 3360000; a(6) = 67200000;
   a(7) = 1344000000; a(8) = 26880000000;
   a(9) = 537600000000; a(10) = 10752000000000.
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;       /* dynamic array of computed values   */
static idx_t  cache_len = 0;      /* allocated size of cache           */
static idx_t  computed_len = 0;   /* number of values actually set     */

static const seq_t init_vals[11] = {
    (seq_t)1ULL,
    (seq_t)21ULL,
    (seq_t)420ULL,
    (seq_t)8400ULL,
    (seq_t)168000ULL,
    (seq_t)3360000ULL,
    (seq_t)67200000ULL,
    (seq_t)1344000000ULL,
    (seq_t)26880000000ULL,
    (seq_t)537600000000ULL,
    (seq_t)10752000000000ULL
};

/* Grow the cache so that indices up to 'need' are addressable. */
static void ensure_cache(idx_t need)
{
    if (cache_len > need)
        return;

    idx_t new_len = need + 1;
    seq_t *new_cache = NULL;

#if defined(__cplusplus)
    new_cache = new seq_t[new_len];
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache)
        abort();
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
#endif

    /* Initialise new entries to zero */
    for (idx_t i = cache_len; i < new_len; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_len = new_len;
}

/* Ensure that the first 11 terms are present in the cache. */
static void ensure_init(void)
{
    if (computed_len >= 11)
        return;

    ensure_cache(10);
    for (idx_t i = computed_len; i < 11; ++i)
        cache[i] = init_vals[i];
    computed_len = 11;
}

/* Public sequence routine.  External linkage. */
seq_t a(idx_t n)
{
    if (n < 11)
        return init_vals[n];

    ensure_cache(n);
    ensure_init();

    if (computed_len > n)
        return cache[n];

    for (idx_t i = computed_len; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 1; j <= 10; ++j)
            sum += cache[i - j];

        if (i == 11)
            cache[i] = 19 * sum - 190 * cache[i - 11] + (seq_t)1;
        else
            cache[i] = 19 * sum - 190 * cache[i - 11];
    }
    computed_len = n + 1;
    return cache[n];
}
