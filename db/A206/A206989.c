/*
 * A206989  Number of n X 3 0..1 arrays avoiding 0 0 0 horizontally and 1 1 1
 *          vertically.
 * https://oeis.org/A206989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) + 11*a(n-3) - 5*a(n-4) + a(n-5) -
 *            3*a(n-6) + a(n-7) for n > 9 Initial values: a(1)=7, a(2)=49,
 *            a(3)=174, a(4)=840, a(5)=3858, a(6)=17460, a(7)=80070,
 *            a(8)=365784, a(9)=1670838
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values for arbitrary n
       (mutable state is allowed as per requirements) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* how many terms are stored (>=1) */
    static idx_t cache_cap  = 0;   /* allocated capacity (>=cache_size) */

    /* guard against invalid indices; leeway for offset */
    if (n < 1) return 0;

    /* if the requested value is already cached, return it immediately */
    if (n <= cache_size && cache != NULL)
        return cache[n - 1];

    /* first time: allocate and initialise the cache with the first 9 terms */
    if (cache == NULL) {
        /* ensure at least a capacity of 16 for small n */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */

        /* base terms (n = 1 .. 9) */
        static const seq_t init[9] = {
            7ULL,         /* a(1) */
            49ULL,        /* a(2) */
            174ULL,       /* a(3) */
            840ULL,       /* a(4) */
            3858ULL,      /* a(5) */
            17460ULL,     /* a(6) */
            80070ULL,     /* a(7) */
            365784ULL,    /* a(8) */
            1670838ULL    /* a(9) */
        };
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_size = 9;
    }

    /* grow the cache if necessary */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;    /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n (if not already cached) */
    if (n > cache_size) {
        /* start from the first n that requires the recurrence */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            if (k <= 9) continue;           /* base terms already present */

            idx_t idx = k - 1; /* zero‑based index for the new term */

            /* use a temporary variable to avoid overflow of intermediate
               intermediate results in case seq_t is a small unsigned type */
            seq_t t = cache[idx - 1] * 3;
            t += cache[idx - 2] * 5;
            t += cache[idx - 3] * 11;
            t += cache[idx - 5];
            t += cache[idx - 7];
            t -= cache[idx - 4] * 5;
            t -= cache[idx - 6] * 3;

            cache[idx] = t;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
