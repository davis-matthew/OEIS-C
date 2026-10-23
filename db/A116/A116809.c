/*
 * A116809  Number of permutations of length n which avoid the patterns 1432,
 *          2134, 2143.
 * https://oeis.org/A116809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 13*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n >= 6, with a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=76
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache: 1‑indexed, cache[0] unused */
static seq_t *cache = NULL;
static size_t cap   = 0;           /* allocated size (>= indices 0..cap-1) */
static idx_t max_done = 0;         /* largest n computed so far */

/* enlarge cache to contain indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);      /* allocation failure */
        cache = new_cache;
        cap   = new_cap;
        /* new slots are uninitialised – will be written by the loop below */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 – safe fallback */

    /* initialise base values once */
    if (cap < 6) {
        ensure_cache(5);
        cache[0] = 0;                /* unused slot */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)76;
        max_done = 5;
    }

    if (n <= max_done) return cache[n];

    ensure_cache(n);
    for (idx_t i = max_done + 1; i <= n; ++i) {
        /* a(i) = 6*a(i-1) -12*a(i-2) +13*a(i-3) -5*a(i-4) + a(i-5) */
        seq_t t = cache[i-1] * (seq_t)6
                + cache[i-3] * (seq_t)13
                + cache[i-5];
        seq_t sub = cache[i-2] * (seq_t)12
                  + cache[i-4] * (seq_t)5;
        cache[i] = t - sub;
    }
    max_done = n;
    return cache[n];
}
