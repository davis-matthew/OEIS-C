/*
 * A206201  Number of (n+1)X4 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-3)+5*a(n-4)+a(n-5)+2*a(n-8)+2*a(n-10) for n>18
 *            Base values: a(1)=1768 a(2)=3008 a(3)=5136 a(4)=11832 a(5)=27400
 *            a(6)=60800 a(7)=145208 a(8)=346080 a(9)=775944 a(10)=1799656
 *            a(11)=4265112 a(12)=9666256 a(13)=22287416 a(14)=52569856
 *            a(15)=120132224 a(16)=276283624 a(17)=648644008 a(18...
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 45, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>        /* calloc, realloc */

static const seq_t init_vals[19] = {
    0ULL,          /* unused, index 0 */
    1768ULL,       /* a(1) */
    3008ULL,       /* a(2) */
    5136ULL,       /* a(3) */
    11832ULL,      /* a(4) */
    27400ULL,      /* a(5) */
    60800ULL,      /* a(6) */
    145208ULL,     /* a(7) */
    346080ULL,     /* a(8) */
    775944ULL,     /* a(9) */
    1799656ULL,    /* a(10) */
    4265112ULL,    /* a(11) */
    9666256ULL,    /* a(12) */
    22287416ULL,   /* a(13) */
    52569856ULL,   /* a(14) */
    120132224ULL,  /* a(15) */
    276283624ULL,  /* a(16) */
    648644008ULL,  /* a(17) */
    1490750392ULL  /* a(18) */
};

static seq_t *cache = NULL;      /* dynamic array holding already computed values */
static size_t cache_cap = 0;     /* allocated number of elements (index 0..cap-1) */
static size_t curr_max = 0;      /* largest index for which a(index) is known */

seq_t a(idx_t n)
{
    /* 1‑based indexing: a(1) is the first term.  For n==0 return 0. */
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = (size_t)18 + 1;          /* indices 0..18 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return 0;        /* allocation failure */

        /* store the base terms */
        for (size_t i = 1; i <= 18; ++i)
            cache[i] = init_vals[i];
        curr_max = 18;
    }

    /* return a base term directly if requested */
    if (n <= 18)
        return init_vals[(size_t)n];

    /* ensure the array is large enough to hold index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure */
        /* initialise new entries to zero (they will be overwritten) */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n (monotonically increasing) */
    if ((size_t)n > curr_max) {
        for (size_t i = curr_max + 1; i <= (size_t)n; ++i) {
            /* recurrence valid for i > 18; i ≥ 19 so all indices needed are ≤ i-3 */
            cache[i] = 10ULL * cache[i - 3]
                     + 5ULL  * cache[i - 4]
                     + cache[i - 5]
                     + 2ULL  * cache[i - 8]
                     + 2ULL  * cache[i -10];
        }
        curr_max = (size_t)n;
    }

    return cache[(size_t)n];
}
