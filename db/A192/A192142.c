/*
 * A192142  1-sequence of reduction of (n^2+n+1) by x^2 -> x+1.
 * https://oeis.org/A192142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 5*a(n-3) + 5*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) - a(n-7) with initial values a(1)=0, a(2)=3, a(3)=10,
 *            a(4)=36, a(5)=99, a(6)=254, a(7)=598
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* maximum index already computed */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* guard against zero index (not part of the sequence) */
    if (n==0) return 0;

    /* first call: allocate minimal capacity and initialise the 7 base terms */
    if (cache_len == 0) {
        cache_cap = 8;                     /* need indices 0..7 inclusive */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failed – return 0 */

        /* base values (index 0 is unused) */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 3;
        cache[3] = 10;
        cache[4] = 36;
        cache[5] = 99;
        cache[6] = 254;
        cache[7] = 598;
        cache_len = 7;
    }

    /* enlarge cache if necessary */
    if ((idx_t)n > cache_len) {
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* failed enlarge → return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing values using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* positive terms */
            seq_t s1 = mul_small(cache[i-1], 4);
            seq_t s2 = mul_small(cache[i-4], 5);
            seq_t s3 = mul_small(cache[i-5], 3);
            seq_t s  = s1 + s2 + s3;

            /* negative terms */
            seq_t t1 = mul_small(cache[i-2], 3);
            seq_t t2 = mul_small(cache[i-3], 5);
            seq_t t3 = mul_small(cache[i-6], 2);
            seq_t t4 = cache[i-7];
            seq_t t  = t1 + t2 + t3 + t4;

            cache[i] = s - t;   /* s >= t for all n >= 8 */
        }
        cache_len = n;
    }

    return cache[n];
}
