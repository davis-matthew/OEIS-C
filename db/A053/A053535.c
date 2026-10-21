/*
 * A053535  Expansion of 1/((1+3*x)*(1-9*x)).
 * https://oeis.org/A053535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) + 27*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache of computed terms  */
static size_t a_cache_size = 0; /* number of terms currently stored */
static size_t a_cache_cap   = 0; /* allocated capacity */

/* Ensure that a_cache has values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (a_cache_size > n)
        return;

    /* Grow capacity if necessary */
    if (a_cache_cap == 0) {
        a_cache_cap = 8;
    }
    while (a_cache_cap <= (size_t)n) {
        a_cache_cap <<= 1;
    }
    a_cache = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));

    /* Fill cache up to n */
    for (size_t i = a_cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            a_cache[i] = (seq_t)1;
        } else if (i == 1) {
            a_cache[i] = (seq_t)6;
        } else {
            a_cache[i] = (seq_t)((seq_t)6 * a_cache[i-1] + (seq_t)27 * a_cache[i-2]);
        }
    }
    a_cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
