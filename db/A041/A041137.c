/*
 * A041137  Denominators of continued fraction convergents to sqrt(77).
 * https://oeis.org/A041137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 702*a(n-6) - a(n-12) for n >= 12 initial values: a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=9, a(4)=31, a(5)=40, a(6)=671, a(7)=711,
 *            a(8)=2804, a(9)=6319, a(10)=21761, a(11)=28080
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence element type */
 /* (seq_t is defined by the build) */

/* Sequence index type */
 /* (idx_t is defined by the build) */

static const seq_t INIT[12] = {
    (seq_t)1, (seq_t)1, (seq_t)4,  (seq_t)9,  (seq_t)31,
    (seq_t)40, (seq_t)671, (seq_t)711, (seq_t)2804, (seq_t)6319,
    (seq_t)21761, (seq_t)28080
};

static seq_t *cache = NULL;
static idx_t  cache_len = 0;  /* number of entries currently cached (cache[0..cache_len-1]) */

/* Return the n-th denominator of the continued‑fraction convergents to sqrt(77). */
seq_t a(idx_t n)
{
    /* Handle the initial segment directly. */
    if (n < 12) {
        return INIT[(size_t)n];
    }

    /* Allocate and initialise the cache if this is the first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory – return zero as a graceful fail */
        for (idx_t i = 0; i < 12; ++i) {
            cache[i] = INIT[i];
        }
        cache_len = 12;
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Grow the cache to accommodate the requested index. */
    size_t newlen = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0; /* realloc failure – return zero */
    cache = tmp;

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i >= 12 here, so both i-6 and i-12 are valid indices. */
        seq_t prod = cache[(size_t)(i - 6)] * (seq_t)702;
        cache[(size_t)i] = prod - cache[(size_t)(i - 12)];
    }
    cache_len = newlen;

    return cache[(size_t)n];
}
