/*
 * A207951  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+8*a(n-2)-18*a(n-3)-27*a(n-4)+72*a(n-5)+48*a(n-6)
 *            -168*a(n-7)-42*a(n-8)+252*a(n-9)-252*a(n-11)+42*a(n-12)
 *            +168*a(n-13)-48*a(n-14)-72*a(n-15)+27*a(n-16)+18*a(n-17)
 *            -8*a(n-18)-2*a(n-19)+a(n-20) Initial values for 1 ≤ n ≤ 20:
 *            16,256,1232,5929,18172,55696,133812,321489, 662256,1364224,...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required for the implementation */
#include <stdlib.h>
#include <string.h>
/* Utility for multiplication by a small unsigned integer */
#include "../utils/mul_small.h"

/* Computes the n-th term of OEIS sequence A207951 */
seq_t a(idx_t n)
{
    /* Static cache to store computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of terms currently stored (>=20 after init) */
    static idx_t cache_cap = 0;   /* Current allocation size */

    /* Initial values for n = 1..20.  These are hard‑coded but are part of
       the definition of the sequence. */
    static const seq_t init_vals[20] = {
        16ULL,        256ULL,       1232ULL,      5929ULL,      18172ULL,
        55696ULL,     133812ULL,    321489ULL,    662256ULL,    1364224ULL,
        2526384ULL,   4678569ULL,   8011752ULL,   13719616ULL,  22123992ULL,
        35676729ULL,  54856032ULL,  84345856ULL, 124764640ULL, 184552225ULL
    };

    /* Lazily initialize the cache with the first 20 terms. */
    if (cache == NULL) {
        cache_cap = 20;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failure – return zero (sequence is not computable). */
            return 0;
        }
        memcpy(cache, init_vals, 20 * sizeof(seq_t));
        cache_len = 20;
    }

    if (n == 0)   /* sequence is defined from n=1; n=0 is meaningless */
        return 0;

    /* If the requested term is already in the cache, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure that the cache is large enough to hold terms up to n. */
    if (n > cache_cap) {
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – return zero. */
            return 0;
        }
        cache = tmp;
    }

    /* Compute terms sequentially from cache_len+1 up to n using the
       20‑term recurrence.  The mapping from a(k‑t) to cache indices is
       a(k‑t) -> cache[(k‑t)‑1]. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of negative contributions */

        /* Positive coefficients */
        pos += mul_small(cache[k - 2], 2);   /* a(k-1) */
        pos += mul_small(cache[k - 3], 8);   /* a(k-2) */
        pos += mul_small(cache[k - 6], 72);  /* a(k-5) */
        pos += mul_small(cache[k - 7], 48);  /* a(k-6) */
        pos += mul_small(cache[k -10], 252); /* a(k-9) */
        pos += mul_small(cache[k -13], 42);  /* a(k-12) */
        pos += mul_small(cache[k -14], 168); /* a(k-13) */
        pos += mul_small(cache[k -17], 27);  /* a(k-16) */
        pos += mul_small(cache[k -18], 18);  /* a(k-17) */
        pos += mul_small(cache[k -21], 1);   /* a(k-20) */

        /* Negative coefficients */
        neg += mul_small(cache[k - 4], 18);   /* a(k-3) */
        neg += mul_small(cache[k - 5], 27);   /* a(k-4) */
        neg += mul_small(cache[k - 8], 168);  /* a(k-7) */
        neg += mul_small(cache[k - 9], 42);   /* a(k-8) */
        neg += mul_small(cache[k -12], 252);  /* a(k-11) */
        neg += mul_small(cache[k -15], 48);   /* a(k-14) */
        neg += mul_small(cache[k -16], 72);   /* a(k-15) */
        neg += mul_small(cache[k -19], 8);    /* a(k-18) */
        neg += mul_small(cache[k -20], 2);    /* a(k-19) */

        /* Final value: positives minus negatives */
        seq_t val = pos - neg;

        /* Store the computed term */
        cache[k - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
