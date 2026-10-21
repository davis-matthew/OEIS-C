/*
 * A229696  Number of defective 3-colorings of a 3 X n 0..2 array connected
 *          horizontally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 81*a(n-2) + 185*a(n-3) - 162*a(n-4) +
 *            60*a(n-5) - 8*a(n-6) for n > 10 Initial values: a(1)=0, a(2)=40,
 *            a(3)=626, a(4)=5077, a(5)=35825, a(6)=230383, a(7)=1400413,
 *            a(8)=8184819, a(9)=46455457, a(10)=257724695
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[11] = {
    0,          /* a(0) unused */
    0,          /* a(1) */
    40,         /* a(2) */
    626,        /* a(3) */
    5077,       /* a(4) */
    35825,      /* a(5) */
    230383,     /* a(6) */
    1400413,    /* a(7) */
    8184819,    /* a(8) */
    46455457,   /* a(9) */
    257724695   /* a(10) */
};

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cached_n  = 0;
    static size_t cache_size = 0;

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_size = 32;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;  /* failure, unlikely */

        /* load the first 10 terms (indices 1..10) */
        for (idx_t i = 1; i <= 10; ++i) {
            cache[i] = init[i];
        }
        cached_n = 10;
    }

    /* if requested term already computed, return it */
    if (n <= cached_n) {
        return cache[n];
    }

    /* grow cache if necessary */
    if ((idx_t)cache_size < n + 1) {
        size_t new_cap = cache_size;
        while ((idx_t)new_cap < n + 1) new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_ptr == NULL) return 0;  /* allocation failure */
        cache = new_ptr;
        cache_size = new_cap;
    }

    /* compute terms from cached_n+1 up to n using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        seq_t pos = (seq_t)15 * a1 + (seq_t)185 * a3 + (seq_t)60 * a5;
        seq_t neg = (seq_t)81 * a2 + (seq_t)162 * a4 + (seq_t)8  * a6;

        cache[i] = pos - neg;   /* pos ≥ neg for all n ≥ 11 */
    }
    cached_n = n;
    return cache[n];
}
