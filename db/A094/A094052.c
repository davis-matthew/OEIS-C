/*
 * A094052  Number of walks of length n between two adjacent nodes in the
 *          cycle graph C_7.
 * https://oeis.org/A094052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-4) for n ≥ 4 initial
 *            values: a(0) = 0, a(1) = 1, a(2) = 0, a(3) = 3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for already-computed terms */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* allocated capacity of the cache */

/* Ensure cache has room for terms 0..n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n) return;          /* enough already */

    size_t needed = (size_t)(n + 1);
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed) new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                         /* out of memory */
    cache = tmp;
    cache_cap = new_cap;

    /* Initialise the first four terms (they are constants) */
    if (cache_cap >= 4) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) {
        switch ((int)n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)0;
        case 3: return (seq_t)3;
        default: /* unreachable */ break;
        }
    }

    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        seq_t v = cache[i - 1];           /* a(i-1) */
        v += cache[i - 2] * (seq_t)4;     /* + 4*a(i-2) */
        v -= cache[i - 3] * (seq_t)3;     /* - 3*a(i-3) */
        v -= cache[i - 4] * (seq_t)2;     /* - 2*a(i-4) */
        cache[i] = v;
    }
    return cache[n];
}
