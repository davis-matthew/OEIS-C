/*
 * A186528  Number of (n+1)X2 0..2 arrays with no 2X2 subblock determinant
 *          equal to any horizontal or vertical neighbor 2X2 subblock
 *          determinant.
 * https://oeis.org/A186528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186528: Number of (n+1)×2 0..2 arrays with the determinant
   condition.  The values satisfy the linear recurrence
   a(n) = 3*a(n-1)+17*a(n-2)+66*a(n-3)+129*a(n-4)+183*a(n-5)+162*a(n-6)
          -29*a(n-7)-150*a(n-8)-189*a(n-9)-109*a(n-10)-2*a(n-11)+8*a(n-12)
   for n>13, with initial values a(1) … a(13) given below.  This
   implementation memoises computed terms; it handles any
   idx_t>=1 and any seq_t provided by the build system. */

#include <stdlib.h>
#include <string.h>

static const int coeff[12] = {3, 17, 66, 129, 183, 162, -29, -150, -189, -109, -2, 8};
static const seq_t base[14] = {0,
    81ULL,                         /* a(1) */
    554ULL,                        /* a(2) */
    4020ULL,                       /* a(3) */
    28522ULL,                      /* a(4) */
    203346ULL,                     /* a(5) */
    1448300ULL,                    /* a(6) */
    10317234ULL,                   /* a(7) */
    73494218ULL,                   /* a(8) */
    523535260ULL,                  /* a(9) */
    3729392026ULL,                 /* a(10) */
    26566253962ULL,                /* a(11) */
    189244190416ULL,               /* a(12) */
    1348077312794ULL               /* a(13) */
};

seq_t a(idx_t n)
{
    /* Return 0 for non-positive indices (behaviour below offset not
       important for the challenge). */
    if (n < 1) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index computed, inclusive */
    static size_t cache_cap = 0;   /* array capacity */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 14;               /* we will store indices 0..13 at least */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        memset(cache, 0, cache_cap * sizeof(seq_t));
        /* copy known initial values */
        for (size_t i = 1; i <= 13; ++i) cache[i] = base[i];
        cache_len = 13;
    }

    /* Extend cache capacity if required */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* If term already memoised, return it */
    if ((size_t)n <= cache_len) return cache[n];

    /* Compute missing terms up to n */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        seq_t pos = 0, neg = 0;
        /* a(k) depends on a(k-1) … a(k-12) */
        for (int i = 0; i < 12; ++i) {
            size_t idx = k - 1 - i;          /* a(k-i-1) */
            seq_t term = cache[idx];
            int c = coeff[i];
            if (c >= 0) pos += (seq_t)c * term;
            else        neg += (seq_t)(-c) * term;
        }
        cache[k] = pos - neg;
    }
    cache_len = (size_t)n;
    return cache[n];
}
