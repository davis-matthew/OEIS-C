/*
 * A042788  Numerators of continued fraction convergents to sqrt(925).
 * https://oeis.org/A042788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0..9) = 30,61,152,365,882,53285,107452,268189,643830,1555849
 *            Recurrence for n >= 10: a(n) = 1764 * a(n-5) + a(n-10)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* allocated array of terms                */
static idx_t  cache_len = 0;         /* length of allocated array (in terms)    */
static idx_t  computed  = 0;         /* number of terms that have been computed*/

/* array of the first ten terms (constants) */
static const seq_t init_vals[10] =
{
    (seq_t)30,     (seq_t)61,     (seq_t)152,    (seq_t)365,    (seq_t)882,
    (seq_t)53285,  (seq_t)107452, (seq_t)268189, (seq_t)643830, (seq_t)1555849
};

/* ensure that the cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (computed > n)                 /* nothing to do */
        return;

    /* grow the cache if necessary ------------------------------------- */
    if (cache_len < n + 1) {
        /* allocate new storage */
        seq_t *new_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_arr) abort();      /* out of memory */

        /* copy old values to new array */
        for (idx_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];

        /* free old storage and install the new one */
        free(cache);
        cache = new_arr;
        cache_len = n + 1;
    }

    /* compute missing terms ------------------------------------------- */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 10) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i - 5] * (seq_t)1764 + cache[i - 10];
        }
    }

    computed = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
