/*
 * A042321  Denominators of continued fraction convergents to sqrt(687).
 * https://oeis.org/A042321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 330674 * a(n-10) - a(n-20)
    a(0) = 1
    a(1) = 4
    a(2) = 5
    a(3) = 14
    a(4) = 19
    a(5) = 318
    a(6) = 337
    a(7) = 992
    a(8) = 1329
    a(9) = 6308
    a(10) = 329345
    a(11) = 1323688
    a(12) = 1653033
    a(13) = 4629754
    a(14) = 6282787
    a(15) = 105154346
    a(16) = 111437133
    a(17) = 328028612
    a(18) = 439465745
    a(19) = 2085891592
*/

#include <stdbool.h>
#include <stdlib.h>

/* Initial 20 terms of the sequence */
static const seq_t init_vals[20] = {
    1ULL,           /* a(0) */
    4ULL,           /* a(1) */
    5ULL,           /* a(2) */
    14ULL,          /* a(3) */
    19ULL,          /* a(4) */
    318ULL,         /* a(5) */
    337ULL,         /* a(6) */
    992ULL,         /* a(7) */
    1329ULL,        /* a(8) */
    6308ULL,        /* a(9) */
    329345ULL,      /* a(10) */
    1323688ULL,     /* a(11) */
    1653033ULL,     /* a(12) */
    4629754ULL,     /* a(13) */
    6282787ULL,     /* a(14) */
    105154346ULL,   /* a(15) */
    111437133ULL,   /* a(16) */
    328028612ULL,   /* a(17) */
    439465745ULL,   /* a(18) */
    2085891592ULL   /* a(19) */
};

static seq_t *cache      = NULL;     /* dynamic array for computed terms */
static size_t cache_cap  = 0;         /* allocated capacity of cache */
static idx_t  max_computed = (idx_t)-1;/* highest n computed so far */

/* Ensure the cache has space for at least n+1 entries */
static void ensure_capacity(idx_t n) {
    if ((idx_t)cache_cap > n + 1) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 20;
    while (new_cap <= (size_t)n + 1) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the cache with the first 20 known values */
static void init_cache(void) {
    ensure_capacity(19);
    for (idx_t i = 0; i < 20; ++i) cache[i] = init_vals[i];
    max_computed = 19;
}

/* Compute a(n) using the recurrence a(n)=330674*a(n-10)-a(n-20) */
seq_t a(idx_t n) {
    if (!cache) init_cache();

    if ((idx_t)cache_cap <= n) {
        ensure_capacity(n);
    }

    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i < 20) {
            /* Should not happen because cache already contains first 20 terms */
            cache[i] = init_vals[i];
        } else {
            seq_t v1 = cache[i - 10];
            seq_t v2 = cache[i - 20];
            cache[i] = (seq_t)330674 * v1 - v2;
        }
    }
    max_computed = n;
    return cache[n];
}
