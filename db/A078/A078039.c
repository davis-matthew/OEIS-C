/*
 * A078039  Expansion of (1 - x)/(1 + x - 2*x^2 + x^3).
 * https://oeis.org/A078039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) - a(n-3) with a(0)=1, a(1)=-2, a(2)=4
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 28, i64 n <= 57, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_size = 0;
static idx_t capacity = 0;

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialize with first three terms */
        capacity = 8;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)4;
        cached_size = 3;
    }
    if (n < cached_size)
        return;
    /* grow capacity if needed */
    while (capacity <= n) {
        capacity = capacity * 2 + 1; /* increase at least once */
    }
    if (capacity > cached_size) {
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }
    /* compute missing values */
    for (idx_t i = cached_size; i <= n; ++i) {
        seq_t v = -cache[i - 1] + (seq_t)2 * cache[i - 2] - cache[i - 3];
        cache[i] = v;
    }
    cached_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
