/*
 * A078005  Expansion of (1-x)/(1-2*x+2*x^2+2*x^3).
 * https://oeis.org/A078005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+3) = 2*a(n+2) - 2*a(n+1) - 2*a(n), a(0)=1, a(1)=1, a(2)=0
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 34, i64 n <= 72, i128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;

/* Extend the cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)realloc(NULL, 3 * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)0;
        seq_cache_size = 3;
    }
    if (seq_cache_size > n) return;

    /* Grow cache to hold indices 0..n */
    seq_cache = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = seq_cache_size; i <= n; ++i) {
        seq_t v = 2 * seq_cache[i - 1] - 2 * seq_cache[i - 2] - 2 * seq_cache[i - 3];
        seq_cache[i] = v;
    }
    seq_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    ensure_cache(n);
    return seq_cache[n];
}
