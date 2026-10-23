/*
 * A142966  Fourth column (m=3) of triangle A142963 divided by 8.
 * https://oeis.org/A142966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142966
   Recurrence: a(n) = 20a(n-1) - 175a(n-2) + 882a(n-3) - 2835a(n-4)
                 + 6072a(n-5) - 8777a(n-6) + 8458a(n-7) - 5204a(n-8)
                 + 1848a(n-9) - 288a(n-10)
   Initial values:
     a(0)=1, a(1)=58, a(2)=877, a(3)=8030, a(4)=56285,
     a(5)=335162, a(6)=1792749, a(7)=8904486,
     a(8)=41949645, a(9)=190129090
   The function is memoised and works for any order of calls. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;           /* memoisation array */
static size_t cache_cap = 0;          /* capacity of cache */
static size_t max_n_comp = SIZE_MAX;  /* largest n for which cache[n] is valid */

/* initial values of the sequence (n = 0 .. 9) */
static const seq_t init[10] = {
    (seq_t)1,           (seq_t)58,          (seq_t)877,
    (seq_t)8030,        (seq_t)56285,       (seq_t)335162,
    (seq_t)1792749,     (seq_t)8904486,     (seq_t)41949645,
    (seq_t)190129090
};

/* coefficients of the recurrence (length 10) */
static const int coeffs[10] = {
    20,  -175,   882,  -2835,   6072,
   -8777,  8458, -5204,  1848,  -288
};

seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (cache == NULL) {
        size_t init_cap = 12;  /* at least the first 10 values */
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) { abort(); }
        for (size_t i = 0; i < 10; ++i) cache[i] = init[i];
        cache_cap = init_cap;
        max_n_comp = 9;
    }

    /* if the requested value is already known return it */
    if ((size_t)n <= max_n_comp) {
        return cache[(size_t)n];
    }

    /* enlarge cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) { abort(); }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = max_n_comp + 1; i <= (size_t)n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 10; ++k) {
            seq_t term = cache[i - 1 - k] * (seq_t)(coeffs[k] > 0 ? coeffs[k] : -coeffs[k]);
            if (coeffs[k] > 0)
                pos += term;
            else
                neg += term;
        }
        cache[i] = pos - neg;  /* pos >= neg by the recurrence */
    }

    max_n_comp = (size_t)n;
    return cache[(size_t)n];
}
