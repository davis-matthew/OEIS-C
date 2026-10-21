/*
 * A224371  Number of nX5 0..2 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) -141*a(n-2) +427*a(n-3) -546*a(n-4) +526*a(n-5)
 *            * -2124*a(n-6) +3302*a(n-7) +2396*a(n-8) +7504*a(n-9)
 *            +580*a(n-10) * +1632*a(n-11) for n>13 * Initial values: *
 *            a(1)=86, a(2)=1915, a(3)=26314, a(4)=311367, a(5)=3607078, *
 *            a(6)=42132769, a(7)=495660330, a(8)=5848449149, * a(9)=6906...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

/* Return the n‑th term of OEIS A224371.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache      = NULL;   /* array 1 .. cache_cap-1 */
    static size_t cache_cap  = 0;      /* total allocated slots */
    static idx_t  cache_last = 0;      /* largest n that has been computed */
    static int    init_done  = 0;      /* whether the initial 13 terms are set */

    if (n == 0)               /* out‑of‑range; defined behaviour not required */
        return 0;

    /* Ensure we have space for indices up to n */
    idx_t needed = n + 1;     /* we will use indices 1 .. n inclusive */
    if (needed < 14)          /* need room for the first 13 terms anyway */
        needed = 14;

    if (cache_cap < (size_t)needed) {
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 (caller may handle error) */
            return 0;
        }
        /* Copy previously computed terms, if any */
        if (cache) {
            for (size_t i = 1; i <= cache_last; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache      = new_cache;
        cache_cap  = (size_t)needed;

        if (!init_done) {   /* initialise the first 13 terms once */
            cache[1]  = (seq_t)86;
            cache[2]  = (seq_t)1915;
            cache[3]  = (seq_t)26314;
            cache[4]  = (seq_t)311367;
            cache[5]  = (seq_t)3607078;
            cache[6]  = (seq_t)42132769;
            cache[7]  = (seq_t)495660330;
            cache[8]  = (seq_t)5848449149ULL;
            cache[9]  = (seq_t)69064897862ULL;
            cache[10] = (seq_t)815702088065ULL;
            cache[11] = (seq_t)9633856474182ULL;
            cache[12] = (seq_t)113778532335717ULL;
            cache[13] = (seq_t)1343744395119438ULL;
            cache_last = 13;
            init_done = 1;
        }
    }

    /* If the requested term is already known, return it */
    if (n <= cache_last)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 13) {
            /* Safety check: first 13 terms are already initialised */
            continue;
        }
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        sum_pos = mul_small(cache[i-1], 21);
        sum_neg = mul_small(cache[i-2], 141);
        sum_pos += mul_small(cache[i-3], 427);
        sum_neg += mul_small(cache[i-4], 546);
        sum_pos += mul_small(cache[i-5], 526);
        sum_neg += mul_small(cache[i-6], 2124);
        sum_pos += mul_small(cache[i-7], 3302);
        sum_pos += mul_small(cache[i-8], 2396);
        sum_pos += mul_small(cache[i-9], 7504);
        sum_pos += mul_small(cache[i-10], 580);
        sum_pos += mul_small(cache[i-11], 1632);

        /* Final value is sum_pos - sum_neg; the result is guaranteed to be
           non‑negative for all n. */
        cache[i] = sum_pos - sum_neg;
    }

    cache_last = n;
    return cache[n];
}
