/*
 * A027674  Numerical distance between m-th and (m+n)-th spheres in loxodromic
 *          sequence of spheres in which each 5 consecutive spheres are in
 *          mutual contact.
 * https://oeis.org/A027674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) - a(n-5) a(0) = -1,
 *            a(1) = a(2) = a(3) = a(4) = 1
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 36, i64 n <= 71, i128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static size_t capacity = 0;
static size_t cache_size = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (cache_size == 0) {
        ensure_capacity(5);
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache_size = 5;
    }

    size_t idx = (size_t)n;
    if (idx < cache_size)
        return cache[idx];

    ensure_capacity(idx + 1);

    for (size_t i = cache_size; i <= idx; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] - cache[i-5];
    }
    cache_size = idx + 1;
    return cache[idx];
}
