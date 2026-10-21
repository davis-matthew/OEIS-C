/*
 * A052601  E.g.f. (1-x)/(1-x-2x^3).
 * https://oeis.org/A052601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=0,
   a(n) = (2*(n-3)^3 + 12*(n-3)^2 + 22*(n-3) + 12)*a(n-3) + n*a(n-1)  for n≥3  */
/* OEIS A052601 – E.g.f. (1‑x)/(1‑x‑2x³) */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms                */
static idx_t cache_size = 0;  /* number of valid entries in cache (next free index) */

/* Return the n-th term of the sequence.  Thread‑unsafe but efficient for repeated calls. */
seq_t a(idx_t n)
{
    /* Initialise the cache if this is the first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache_size = 3;
    }

    /* If we already have the desired term, simply return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate indices up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute any new terms that are missing */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* i≥3 because cache_size≥3 after initialisation */
        idx_t k = i - 3;                          /* k = n-3 in the recurrence      */
        idx_t coeff = 2 * k * k * k + 12 * k * k + 22 * k + 12; /* 2k³ + 12k² + 22k + 12 */
        cache[i] = (seq_t)coeff * cache[i - 3] + (seq_t)i * cache[i - 1];
    }

    cache_size = n + 1;
    return cache[n];
}
