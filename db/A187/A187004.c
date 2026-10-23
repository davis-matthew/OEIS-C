/*
 * A187004  Expansion of A(x) = (1 + 2*x^2 + 6*x^3 + 9*x^4 + 8*x^5 + 5*x^6) /
 *          (1 - x - 2*x^2 - 3*x^3 - 3*x^4 - 2*x^5 - x^6).
 * https://oeis.org/A187004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 3*a(n-4) + 2*a(n-5) +
 *            a(n-6); a(1)=1, a(2)=5, a(3)=16, a(4)=41, a(5)=101, a(6)=254
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* array of computed values, index 0 unused */
static idx_t cache_cap = 0;           /* current allocation size  */
static idx_t cache_len = 0;           /* largest n for which cache[n] is valid */

/* Ensure the internal cache can hold values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but safe */

    /* Initialise cache with first 6 terms if not done yet */
    if (cache_len == 0) {
        ensure_capacity(6);
        cache[0] = 0;                 /* unused */
        cache[1] = 1;
        cache[2] = 5;
        cache[3] = 16;
        cache[4] = 41;
        cache[5] = 101;
        cache[6] = 254;
        cache_len = 6;
    }

    /* If requested term already computed, return it */
    if (n <= cache_len) return cache[n];

    /* Grow cache to required size and compute missing terms */
    ensure_capacity(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + 2*a(i-2) + 3*a(i-3) + 3*a(i-4) + 2*a(i-5) + a(i-6) */
        seq_t val = cache[i-1];
        val += (seq_t)2 * cache[i-2];
        val += (seq_t)3 * cache[i-3];
        val += (seq_t)3 * cache[i-4];
        val += (seq_t)2 * cache[i-5];
        val += cache[i-6];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
