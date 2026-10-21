/*
 * A054883  Number of walks of length n along the edges of a dodecahedron
 *          between two opposite vertices.
 * https://oeis.org/A054883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=a(4)=0, a(5)=6 For n≥5: a(n) = 2*a(n-1) +
 *            10*a(n-2) − 16*a(n-3) − 25*a(n-4) + 30*a(n-5) The recurrence
 *            follows from the generating function
 *            6*x⁵/((1–x)(1+2x)(1–3x)(1–5x²)). The right‑hand side is computed
 *            as a difference of two positive sums to avoid subtraction on
 *            unsigned ty...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache       = NULL;   /* memoization buffer */
static idx_t  cache_len   = 0;       /* number of entries currently stored */
static idx_t  cache_cap   = 0;       /* allocated capacity */

/* Ensure the cache holds indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In case realloc fails, the original pointer is unchanged and the
       program will behave correctly for indices already computed.            */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0)=0 */

    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 5) {
            cache[i] = (seq_t)0;
        } else if (i == 5) {
            cache[i] = (seq_t)6;
        } else {
            /* Positive part of the recurrence */
            seq_t pos = (cache[i-1] * (seq_t)2) +
                        (cache[i-2] * (seq_t)10) +
                        (cache[i-5] * (seq_t)30);

            /* Negative part of the recurrence */
            seq_t neg = (cache[i-3] * (seq_t)16) +
                        (cache[i-4] * (seq_t)25);

            /* Guaranteed non‑negative difference */
            cache[i] = pos - neg;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
