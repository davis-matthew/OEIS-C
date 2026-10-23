/*
 * A123184  Expansion of
 *          -x*(3*x^4+9*x^3-29*x^2+12*x-1)/((x-1)*(x^4-3*x^3-27*x^2+12*x-1)).
 * https://oeis.org/A123184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) satisfies a(1)=1, a(2)=1, a(3)=3, a(4)=15, a(5)=103, *
 *            a(n)=13*a(n-1)-39*a(n-2)+24*a(n-3)+4*a(n-4)-a(n-5) for n>=6 *
 *            This recurrence follows from the rational generating function *
 *            -x*(3x^4+9x^3-29x^2+12x-1)/((x-1)*(x^4-3x^3-27x^2+12x-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* cache[0] unused, cache[i] = a(i) */
static size_t cache_size = 0;       /* allocated array length (>= n+1) */
static idx_t   last_computed = 0;   /* largest n for which cache[n] is valid */

static void ensure_cache(idx_t n)
{
    /* allocate or grow cache so it can hold indices up to n */
    if (!cache || (size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size;
        if (new_size == 0) new_size = (n + 1 > 8) ? (n + 1) : 8;
        while ((size_t)(n + 1) > new_size) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise first five terms if they are not yet present */
    if (last_computed < 5) {
        if (last_computed < 1) cache[1] = (seq_t)1;
        if (last_computed < 2) cache[2] = (seq_t)1;
        if (last_computed < 3) cache[3] = (seq_t)3;
        if (last_computed < 4) cache[4] = (seq_t)15;
        if (last_computed < 5) cache[5] = (seq_t)103;
        last_computed = 5;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = (seq_t)13 * a1
                 - (seq_t)39 * a2
                 + (seq_t)24 * a3
                 + (seq_t) 4 * a4
                 - a5;
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset 1: defined for n>=1 only */
    ensure_cache(n);
    return cache[n];
}
