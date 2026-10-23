/*
 * A159679  a(n) are solutions of the 2 equations: 7*a(n) +1 = c(n)^2 and
 *          9*a(n) + 1 = b(n)^2.
 * https://oeis.org/A159679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A159679:  a(n+3) = 255*(a(n+2)-a(n+1)) + a(n),  a(1)=0, a(2)=32, a(3)=8160 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* 1‑based index cache */
static size_t cache_cap = 0;       /* allocated size (excluding index 0) */
static size_t cache_last = 0;      /* highest index computed */

/* Ensure the cache is large enough to store indices up to n. */
static void ensure_cache(size_t n) {
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while (new_cap < n) new_cap <<= 1;          /* grow exponentially */
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort();                    /* out of memory */
    /* initialize newly allocated slots to 0 */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the first three terms if not done already. */
static void init_base(void) {
    if (cache_last) return;
    cache[1] = 0;
    cache[2] = 32;
    cache[3] = 8160;
    cache_last = 3;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined, but keep graceful */
    size_t N = (size_t)n;
    ensure_cache(N);
    init_base();

    if (N <= cache_last) return cache[N];

    for (size_t i = cache_last + 1; i <= N; ++i) {
        seq_t term = ((seq_t)255) * (cache[i - 1] - cache[i - 2]) + cache[i - 3];
        cache[i] = term;
    }
    cache_last = N;
    return cache[N];
}
