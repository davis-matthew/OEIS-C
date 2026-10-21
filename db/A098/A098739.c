/*
 * A098739  This sequence is constructed using only the four single-digit
 *          primes 2,3,5,7 in that order.
 * https://oeis.org/A098739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5) (n>5) Initial values:
 *            a(1)=2, a(2)=23, a(3)=235, a(4)=2357, a(5)=23572
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based indexing, cache[0] unused */
static idx_t cache_size = 0;     /* largest n for which cache[n] is valid */
static idx_t cache_cap = 0;      /* allocated capacity */

/* initialise the cache with first 5 terms */
static void init_cache(void) {
    /* start with a small capacity, grow as needed */
    cache_cap = 32;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;  /* allocation failure – cannot be handled, return 0 */
    cache[0] = 0;                     /* unused, keeps indices positive */
    cache[1] = 2;
    cache[2] = 23;
    cache[3] = 235;
    cache[4] = 2357;
    cache[5] = 23572;
    cache_size = 5;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined, but return 0 gracefully */

    if (!cache) init_cache();        /* initialise on first use */

    /* grow cache if needed */
    while (n > cache_cap) {
        cache_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – return 0 */
    }

    /* if already computed, return */
    if (n <= cache_size) return cache[n];

    /* compute terms up to n using recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t term = cache[i-1] * ((seq_t)10) + cache[i-4]
                     - ((seq_t)10 * cache[i-5]);
        cache[i] = term;
    }
    cache_size = n;
    return cache[n];
}
