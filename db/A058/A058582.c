/*
 * A058582  Expansion of (1+3*x+4*x^2)/(1-4*x^2+4*x^4).
 * https://oeis.org/A058582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 4*a(n-4) for n >= 4, with initial values
 *            a(0)=1, a(1)=3, a(2)=8, a(3)=12. This follows from the rational
 *            generating function (1+3*x+4*x^2)/(1-4*x^2+4*x^4)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cached_size = 0;

/* ensure the cache can hold terms up to n */
static void ensure_cache(idx_t n)
{
    if (n < cached_size) return;

    idx_t new_size = n + 1;
    if (new_size < 16) new_size = 16;

    while (new_size <= n) new_size <<= 1;      /* grow exponentially */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);        /* allocation failed */
    cache = new_cache;
    cached_size = new_size;
}

/* compute and return the nth term */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* initialize base cases only once */
    if (cached_size == 0) return 0; /* should not happen */

    /* fill terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        if (i == 1) { cache[1] = (seq_t)3; continue; }
        if (i == 2) { cache[2] = (seq_t)8; continue; }
        if (i == 3) { cache[3] = (seq_t)12; continue; }

        /* recurrence: a[i] = 4*a[i-2] - 4*a[i-4] */
        seq_t term1 = cache[i-2] * (seq_t)4;
        seq_t term2 = cache[i-4] * (seq_t)4;
        cache[i] = term1 - term2;
    }

    return cache[n];
}
