/*
 * A042168  Numerators of continued fraction convergents to sqrt(609).
 * https://oeis.org/A042168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1211390*a(n-12) - a(n-24) for n ≥ 24, with a(0) = 24,
 *            a(1) = 25, …, a(23) = 733732866049
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;  /* dynamic cache of computed terms   */
static idx_t cache_size  = 0;     /* allocated size of the cache       */
static idx_t last_computed = 23;  /* largest index that has been computed */

/* Initial values a(0)…a(23) */
static const seq_t init_vals[24] = {
    24ULL, 25ULL, 74ULL, 691ULL, 765ULL, 1456ULL,
    9501ULL, 10957ULL, 20458ULL, 195079ULL, 410616ULL, 605695ULL,
    29483976ULL, 30089671ULL, 89663318ULL, 837059533ULL, 926722851ULL,
    1763782384ULL, 11509417155ULL, 13273199539ULL, 24782616694ULL,
    236316749785ULL, 497416116264ULL, 733732866049ULL
};

/* Ensure the cache is large enough to hold index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = cache_size ? cache_size * 2 : 24;
    while (new_size <= n) new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);

    /* Initialise newly allocated slots. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i < 24) new_cache[i] = init_vals[i];
        else        new_cache[i] = 0;
    }

    cache     = new_cache;
    cache_size = new_size;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (!cache) {          /* first call: allocate the minimal cache */
        cache = (seq_t *)malloc(24 * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 24; ++i) cache[i] = init_vals[i];
        cache_size = 24;
    }

    ensure_cache(n);

    if (n <= last_computed) return cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 24) continue;   /* already stored */
        seq_t term12 = cache[i - 12];
        seq_t term24 = cache[i - 24];
        cache[i] = term12 * 1211390ULL - term24;
    }

    last_computed = n;
    return cache[n];
}
