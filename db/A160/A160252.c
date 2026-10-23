/*
 * A160252  Numerator of Hermite(n, 9/29).
 * https://oeis.org/A160252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 9/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=18; a(n)=18*a(n-1)-1682*(n-1)*a(n-2) for n>=2 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

static void grow_cache_to(idx_t n)
{
    if (n < cache_size) return;
    size_t needed = n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }
    if (cache_size == 0) {
        cache[0] = 1;
        cache_size = 1;
    }
    if (cache_size == 1 && needed > 1) {
        cache[1] = 18;
        cache_size = 2;
    }
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[i-1];
        seq_t prev2 = cache[i-2];
        seq_t coeff = (seq_t)(1682 * (i - 1));
        cache[i] = 18 * prev1 - coeff * prev2;
    }
    if (n + 1 > cache_size) cache_size = n + 1;
}

seq_t a(idx_t n)
{
    grow_cache_to(n);
    return cache[n];
}
