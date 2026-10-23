/*
 * A008339  a(1)=1; for n >= 1, a(n+1) = lcm(a(n),n) / gcd(a(n),n).
 * https://oeis.org/A008339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 1, a(n+1) = lcm(a(n),n)/gcd(a(n),n)
 *            (equivalently a(n+1) = a(n)*n/gcd(a(n),n)^2)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = 1 */
static size_t cache_size = 0;        /* allocated size, in slots with index 0 */
static idx_t  computed_upto = 0;     /* highest n whose a(n) is stored */

/* Ensure that the cache can hold at least indices 0..max_needed. */
static void ensure_cache(size_t max_needed) {
    if (cache_size > max_needed) return;
    size_t new_size = cache_size > 0 ? cache_size * 2 : 64;
    while (new_size <= max_needed) new_size *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);     /* allocation failure */
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* offset starts at 1 */

    /* Initialise the cache on first call. */
    if (!cache) {
        cache_size = 64;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0;          /* unused */
        cache[1] = (seq_t)1;   /* a(1) */
        computed_upto = 1;
    }

    /* Grow cache to accommodate index n. */
    ensure_cache((size_t)n);

    /* Compute missing values up to n using the recurrence:
       a(i) = a(i-1) * (i-1) / gcd(a(i-1), i-1)^2  for i ≥ 2. */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        idx_t m = (idx_t)(i - 1);
        seq_t g   = gcd(prev, (seq_t)m);
        seq_t p   = prev / g;
        seq_t q   = (seq_t)m / g;
        cache[(size_t)i] = seq_mul(p, q);
    }
    computed_upto = n;

    return cache[(size_t)n];
}
