/*
 * A011674  A binary m-sequence: expansion of reciprocal of
 *          x^7+x^6+x^5+x^4+x^3+x^2+1.
 * https://oeis.org/A011674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A011674 – binary m‑sequence from primitive polynomial
   x^7 + x^6 + x^5 + x^4 + x^3 + x^2 + 1
   Recurrence (over GF(2)):
        a(n) = a(n‑2) ⊕ a(n‑3) ⊕ a(n‑4) ⊕ a(n‑5) ⊕ a(n‑6) ⊕ a(n‑7)
   Initial values:
        a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1
*/

#include <stdlib.h>

/* static state for memoisation */
static seq_t *cache = NULL;      /* cache[i] holds a(i) for 0 ≤ i < cache_len */
static idx_t   cache_len = 0;   /* number of values computed so far */
static size_t  cache_cap  = 0;   /* allocated capacity of cache */

/* Ensure the cache is initialised with the first 7 terms */
static void ensure_initial(void) {
    if (cache_len == 0) {
        cache_cap = 8;  /* at least space for indices 0..7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);      /* allocation failure – not expected */

        /* initial terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)1;
        cache_len = 7;
    }
}

/* Grow the cache array to at least (n+1) elements */
static void ensure_cache_cap(idx_t n) {
    if (n + 1 <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)(n + 1))
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);  /* allocation failure – not expected */

    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    ensure_initial();

    /* If requested index is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Make sure the cache has room for n */
    ensure_cache_cap(n);

    /* Compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 2) val ^= cache[i - 2];
        if (i >= 3) val ^= cache[i - 3];
        if (i >= 4) val ^= cache[i - 4];
        if (i >= 5) val ^= cache[i - 5];
        if (i >= 6) val ^= cache[i - 6];
        if (i >= 7) val ^= cache[i - 7];
        cache[i] = val;
    }
    cache_len = n + 1;  /* all indices up to n are now computed */
    return cache[n];
}
