/*
 * A077837  Expansion of (1-x)/(1-3*x-3*x^2-2*x^3).
 * https://oeis.org/A077837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=9, a(n)=3*a(n-1)+3*a(n-2)+2*a(n-3) for n>=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1-x)/(1-3*x-3*x^2-2*x^3). */

/* The function below implements the recurrence while lazily
 * computing values and caching them so that calls can be
 * out‑of‑order and still run quickly. */

#include <stdlib.h> /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* memoization storage */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* constants cast to seq_t for portability */
    const seq_t C3 = (seq_t)3;
    const seq_t C2 = (seq_t)2;

    /* Handle small indices without accessing cache */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;

    /* Ensure cache is large enough */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (cache_len < n + 1) {
        /* Initialize first three values if not already done */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)9;
            cache_len = 3;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)2;
            cache_len = 2;
        } else if (cache_len == 2) {
            cache[2] = (seq_t)9;
            cache_len = 3;
        }

        for (size_t i = cache_len; i <= n; ++i) {
            seq_t val = C3 * cache[i - 1] + C3 * cache[i - 2] + C2 * cache[i - 3];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
