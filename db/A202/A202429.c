/*
 * A202429  Number of (n+2)X4 binary arrays avoiding patterns 000 and 001 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 16*a(n-2) + 21*a(n-3) - 13*a(n-4) - 31*a(n-5) *
 *            + 9*a(n-6) + 20*a(n-7) - 12*a(n-8) + a(n-10) * Initial values:
 *            a(1)=390, a(2)=2100, a(3)=10080, a(4)=50310, a(5)=247800, *
 *            a(6)=1226040, a(7)=6056600, a(8)=29936040, a(9)=147935520, *
 *            a(10)=731109730 * The function memoises computed...
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values, 1-based indexing. index 0 unused. */
    static const seq_t init[11] = {
        0,          /* dummy */
        390,        /* a(1) */
        2100,       /* a(2) */
        10080,      /* a(3) */
        50310,      /* a(4) */
        247800,     /* a(5) */
        1226040,    /* a(6) */
        6056600,    /* a(7) */
        29936040,   /* a(8) */
        147935520,  /* a(9) */
        731109730   /* a(10) */
    };

    if (n <= 10) {
        return init[(size_t)n];
    }

    /* Memoisation cache: cache[i] = a(i) for 1 <= i <= cached_size */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* Initialise cache with first ten terms if necessary. */
    if (cached_size < 10) {
        size_t sz = 11; /* 0..10 inclusive */
        if (!cache) {
            cache = (seq_t *)malloc(sz * sizeof(seq_t));
            if (!cache) return (seq_t)0;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
        }
        for (size_t i = 1; i <= 10; ++i) {
            cache[i] = init[i];
        }
        cached_size = 10;
    }

    /* Grow and compute up to n if needed. */
    if (cached_size < n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        for (idx_t k = cached_size + 1; k <= n; ++k) {
            seq_t pos = cache[k - 1];
            pos += 16 * cache[k - 2];
            pos += 21 * cache[k - 3];
            pos += 9  * cache[k - 6];
            pos += 20 * cache[k - 7];
            pos += cache[k - 10];

            seq_t neg = 13 * cache[k - 4];
            neg += 31 * cache[k - 5];
            neg += 12 * cache[k - 8];

            cache[k] = pos - neg;
        }
        cached_size = n;
    }

    return cache[(size_t)n];
}
