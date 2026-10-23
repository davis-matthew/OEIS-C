/*
 * A183399  Half the number of n X 3 binary arrays with no element equal to a
 *          strict majority of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 23*a(n-3) + 15*a(n-4) + 40*a(n-5) - 46*a(n-6)
 *            + 20*a(n-8) - 8*a(n-9) for n>10 Initial values: a(1)=4, a(2)=2,
 *            a(3)=6, a(4)=32, a(5)=110, a(6)=450, a(7)=1680, a(8)=6498,
 *            a(9)=24794, a(10)=95048 The recurrence is empirical, so the
 *            above initial terms are required to start the calc...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache: cache[i] == a(i+1)  (1‑based to 0‑based offset) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

/* 1‑based initial values */
static const seq_t init[10] = {
    4, 2, 6, 32, 110, 450, 1680, 6498, 24794, 95048
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Ensure the cache can hold n terms */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t idx = cache_cnt; idx < (size_t)n; ++idx) {
        size_t term = idx + 1;          /* 1‑based index of a(term) */

        if (term <= 10) {               /* use initial values */
            cache[idx] = init[term - 1];
            continue;
        }

        /* Recurrence: a(term) = 5*a(term-1) - 23*a(term-3) + 15*a(term-4)
                         + 40*a(term-5) - 46*a(term-6) + 20*a(term-8)
                         - 8*a(term-9) */
        seq_t val = 5 * cache[term - 2];          /* 5*a(term-1) */
        val -= 23 * cache[term - 4];             /* -23*a(term-3) */
        val += 15 * cache[term - 5];             /* +15*a(term-4) */
        val += 40 * cache[term - 6];             /* +40*a(term-5) */
        val -= 46 * cache[term - 7];             /* -46*a(term-6) */
        val += 20 * cache[term - 9];             /* +20*a(term-8) */
        val -= 8 * cache[term - 10];             /* -8*a(term-9) */

        cache[idx] = val;
    }

    cache_cnt = (size_t)n;
    return cache[n - 1];
}
