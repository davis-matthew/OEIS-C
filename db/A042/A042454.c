/*
 * A042454  Numerators of continued fraction convergents to sqrt(755).
 * https://oeis.org/A042454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2418*a(n-4) - a(n-8);  a(0)=27, a(1)=55, a(2)=577, a(3)=1209,
   a(4)=65863, a(5)=132935, a(6)=1395213, a(7)=2923361 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* dynamic cache of computed terms */
static size_t cache_size = 0;  /* allocated size of cache */
static idx_t init_done = 0;    /* number of terms already initialized */

static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n) return;              /* already enough */
    size_t new_size = (size_t)n + 1;                /* we need indices 0..n */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }
    cache_size = new_size;
}

static const seq_t init_vals[8] = {
    27,           /* a(0) */
    55,           /* a(1) */
    577,          /* a(2) */
    1209,         /* a(3) */
    65863,        /* a(4) */
    132935,       /* a(5) */
    1395213,      /* a(6) */
    2923361       /* a(7) */
};

seq_t a(idx_t n)
{
    /* Compute a(n) using the linear recurrence of order 8 */
    ensure_cache(n);              /* make cache large enough */

    /* Fill missing terms up to n */
    for (idx_t i = init_done; i <= n; i++) {
        if (i < 8) {
            cache[i] = init_vals[(size_t)i];
        } else {
            /* a(i) = 2418 * a(i-4) - a(i-8) */
            cache[i] = (seq_t)2418 * cache[(size_t)(i - 4)] - cache[(size_t)(i - 8)];
        }
    }
    init_done = (size_t)(n + 1);

    return cache[(size_t)n];
}
