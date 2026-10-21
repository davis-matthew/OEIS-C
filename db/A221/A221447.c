/*
 * A221447  Number of 2 X n binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 5*a(n-3) + a(n-4) + a(n-5) - a(n-6) -
 *            a(n-7) Initial values: a(1)=1, a(2)=9, a(3)=33, a(4)=117,
 *            a(5)=429, a(6)=1577, a(7)=5785
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n-th term of OEIS A221447 (n >= 1). */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* allocated size (index 0..cache_cap-1) */
    static size_t computed_until = 0;   /* highest index computed so far */

    /* initialize cache if needed */
    if (!cache) {
        /* ensure capacity for at least n+1 (index 0 unused) and room for first 7 terms */
        cache_cap = (n > 7 ? n + 1 : 8);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) { /* out‑of‑memory: abort device‑wide (no errno check) */
            abort();
        }
        /* set first seven terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)33;
        cache[4] = (seq_t)117;
        cache[5] = (seq_t)429;
        cache[6] = (seq_t)1577;
        cache[7] = (seq_t)5785;
        computed_until = 7;
    }
    /* grow cache if the caller asks for a larger index */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    /* if the value is already computed, just return it */
    if (n <= computed_until) {
        return cache[n];
    }
    /* compute terms up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1];
        val += cache[i - 2];
        val += 5 * cache[i - 3];
        val += cache[i - 4];
        val += cache[i - 5];
        val -= cache[i - 6];
        val -= cache[i - 7];
        cache[i] = val;
    }
    computed_until = n;
    return cache[n];
}
