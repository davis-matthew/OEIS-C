/*
 * A220762  Number of 2Xn arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random 0..3 2Xn array.
 * https://oeis.org/A220762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +13*a(n-2) -10*a(n-3) +12*a(n-4) -41*a(n-5) *
 *            +6*a(n-6) -16*a(n-7) -40*a(n-8) +4*a(n-9) +64*a(n-11)
 *            +32*a(n-12), * a(1)=16, a(2)=28, a(3)=268, a(4)=708, a(5)=4766,
 *            a(6)=15724, a(7)=88434, * a(8)=331024, a(9)=1683150,
 *            a(10)=6777522, a(11)=32549034, a(12)=136712744
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* highest index already computed */

    if (n == 0) {                        /* out of offset range - return 0 */
        return 0;
    }

    /* initial values (1‑based indexing, cache[0] unused) */
    static const seq_t init[13] = {
        0ULL,        /* 0th unused */
        16ULL,       /* a(1) */
        28ULL,       /* a(2) */
        268ULL,      /* a(3) */
        708ULL,      /* a(4) */
        4766ULL,     /* a(5) */
        15724ULL,    /* a(6) */
        88434ULL,    /* a(7) */
        331024ULL,   /* a(8) */
        1683150ULL,  /* a(9) */
        6777522ULL,  /* a(10) */
        32549034ULL, /* a(11) */
        136712744ULL /* a(12) */
    };

    /* expand cache if necessary */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – fall back to non‑cached evaluation */
            new_cache = cache; /* keep old cache to avoid memory leak */
        }
        cache = new_cache;
        /* initialise new entries to 0 (they will be overwritten below) */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = 0;
        }

        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                /* positive terms */
                seq_t pos  = 2 * cache[i - 1]      /* 2*a(n-1) */
                           + 13 * cache[i - 2]    /* 13*a(n-2) */
                           + 12 * cache[i - 4]    /* 12*a(n-4) */
                           + 6  * cache[i - 6]    /* 6*a(n-6) */
                           + 4  * cache[i - 9]    /* 4*a(n-9) */
                           + 64 * cache[i - 11]   /* 64*a(n-11) */
                           + 32 * cache[i - 12];  /* 32*a(n-12) */

                /* negative terms */
                seq_t neg  = 10 * cache[i - 3]    /* 10*a(n-3) */
                           + 41 * cache[i - 5]    /* 41*a(n-5) */
                           + 16 * cache[i - 7]    /* 16*a(n-7) */
                           + 40 * cache[i - 8];   /* 40*a(n-8) */

                cache[i] = pos - neg;  /* unsigned subtraction is safe because
                                         * recurrence guarantees pos >= neg. */
            }
        }
        cache_len = n;
    }

    return cache[n];
}
