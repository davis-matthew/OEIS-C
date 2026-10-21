/*
 * A075925  Sixth column of triangle A075502.
 * https://oeis.org/A075925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n * S2(n+6,6) (Stirling numbers of the second kind).
 *            Recurrence: a(n) = 147 a(n‑1) – 8575 a(n‑2) + 252105 a(n‑3) –
 *            3899224 a(n‑4) + 29647548 a(n‑5) – 84707280 a(n‑6), for n ≥ 6,
 *            with a(0)=1, a(1)=147, a(2)=13034, a(3)=907578, a(4)=54807627,
 *            a(5)=3016638009
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* array of computed terms   */
static size_t cache_cap   = 0;      /* allocated capacity        */
static size_t cache_len   = 0;      /* number of terms stored    */

/* initialise the static cache with the first six values */
static void init_cache(void) {
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)147;
        cache[2] = (seq_t)13034;
        cache[3] = (seq_t)907578;
        cache[4] = (seq_t)54807627;
        cache[5] = (seq_t)3016638009;
        cache_len = 6;
    }
}

/* extend the cache so that cache[n] is valid */
static void extend_cache(idx_t n) {
    init_cache();
    if (n < cache_len)
        return;

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term  = (seq_t)147 * cache[i-1];
        term -= (seq_t)8575 * cache[i-2];
        term += (seq_t)252105 * cache[i-3];
        term -= (seq_t)3899224 * cache[i-4];
        term += (seq_t)29647548 * cache[i-5];
        term -= (seq_t)84707280 * cache[i-6];
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n) {
    extend_cache(n);
    return cache[n];
}
