/*
 * A231998  Number of (1+1) X (n+1) 0..1 arrays with no element having a
 *          strict majority of its horizontal and antidiagonal neighbors equal
 *          to one.
 * https://oeis.org/A231998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = a(n-1) + 3*a(n-3) + 3*a(n-4) + 3*a(n-5) + 3*a(n-6)
           - 2*a(n-8) - a(n-9)
   for n ≥ 10.
   Initial values:
   a(1)=4, a(2)=6, a(3)=16, a(4)=39,
   a(5)=81, a(6)=168, a(7)=361,
   a(8)=780, a(9)=1681. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached values a(1)...a(k) */
    static size_t cached = 0;      /* number of terms stored */
    static size_t cap = 0;         /* allocated capacity */

    /* Base cases up to n = 9 */
    static const seq_t base[9] = {
        4, 6, 16, 39, 81, 168, 361, 780, 1681
    };

    /* Convert idx_t to a platform‐independent size_t for array indexing */
    size_t N = (size_t)n;
    if (N == 0) return (seq_t)0;   /* undefined for n <= 0, return 0 */

    /* Lazy initialization on first call */
    if (cache == NULL) {
        /* Allocate an initial capacity of at least 9 */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base[i];
        cached = 9;
    }

    /* If requested term already computed, return it */
    if (N <= cached)
        return cache[N - 1];

    /* Need to compute terms up to N */
    if (N > cap) {   /* grow capacity if necessary (doubling strategy) */
        size_t newcap = cap;
        while (newcap < N)
            newcap <<= 1;   /* double until sufficient */
        seq_t *newcache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newcache) abort();  /* out of memory */
        cache = newcache;
        cap = newcap;
    }

    /* Compute new terms using the recurrence */
    for (size_t k = cached; k < N; ++k) {
        /* k corresponds to a(k+1) */
        seq_t val = cache[k - 1];           /* a(k) = a((k+1)-1)   */
        val += cache[k - 3] * ((seq_t)3);   /* 3*a((k+1)-3) */
        val += cache[k - 4] * ((seq_t)3);   /* 3*a((k+1)-4) */
        val += cache[k - 5] * ((seq_t)3);   /* 3*a((k+1)-5) */
        val += cache[k - 6] * ((seq_t)3);   /* 3*a((k+1)-6) */
        val -= cache[k - 8] * ((seq_t)2);   /* -2*a((k+1)-8) */
        val -= cache[k - 9];                /* -a((k+1)-9)     */
        cache[k] = val;
    }

    cached = N;  /* update cache size */
    return cache[N - 1];
}
