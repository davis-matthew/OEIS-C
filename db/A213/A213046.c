/*
 * A213046  Convolution of Lucas numbers and positive integers repeated
 *          (A000032 and A008619).
 * https://oeis.org/A213046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) + a(n-5) Initial values:
 *            a(0)=2, a(1)=3, a(2)=8, a(3)=13, a(4)=25 (Indices start at 0)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* sequence cache */
static size_t cap   = 0;      /* allocated capacity of cache */
static idx_t  last  = 4;      /* highest index already computed in cache */

/* Ensure the cache has space for indices up to n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t new_cap = cap ? cap : 5;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }
}

/* Return the n‑th term of A213046 */
seq_t a(idx_t n)
{
    /* Initialise the cache once */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        cache[0] = 2;
        cache[1] = 3;
        cache[2] = 8;
        cache[3] = 13;
        cache[4] = 25;
        cap   = 5;
        last  = 4;
    }

    /* For indices below 5 the initial values are known */
    if (n < 5) {
        return cache[n];
    }

    /* Grow cache if necessary */
    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = 0;
        term += cache[i - 1] * 2;          /* 2*a(i-1) */
        term += cache[i - 2];              /* + a(i-2) */
        term -= cache[i - 3] * 3;          /* - 3*a(i-3) */
        term += cache[i - 5];              /* + a(i-5) */
        cache[i] = term;
    }
    last = n;

    return cache[n];
}
