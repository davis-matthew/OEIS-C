/*
 * A159666  a(n) = Sum_{k=1..n} binomial(n,k) * gcd(n,k) * a(k-1), with a(0) =
 *          1.
 * https://oeis.org/A159666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * gcd(n,k) * a(k-1), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>          /* for malloc, free if needed */

#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;      /* array of a(0) .. a(current_max) */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static idx_t cache_last = 0;     /* last index computed (>=0 if cache non-empty) */

    /* Ensure capacity for n+1 elements */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < n + 1) new_cap *= 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Initialize a(0) if needed */
    if (cache_last == 0 && (cache_cap == 0 || cache[0] == 0)) {
        cache[0] = (seq_t)1;
        cache_last = 0;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 0) {                /* already initialized above */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        seq_t binom = 1;             /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binom to C(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* Compute term: C(i,k) * gcd(i,k) * a(k-1) */
            seq_t g   = gcd((seq_t)i, (seq_t)k);
            seq_t term = binom * g * cache[k - 1];
            sum += term;
        }

        cache[i] = sum;
        cache_last = i;
    }

    return cache[n];
}
