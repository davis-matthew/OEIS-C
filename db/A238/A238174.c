/*
 * A238174  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A238174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) -111*a(n-2) -106*a(n-3) +1473*a(n-4)
 *            -2198*a(n-5) -121*a(n-6) +1514*a(n-7) -472*a(n-8) Initial terms:
 *            a(1)=423, a(2)=5321, a(3)=69023, a(4)=938601, a(5)=12951743,
 *            a(6)=180345769, a(7)=2520461647, a(8)=35289713177
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Computes a(n) for the OEIS sequence A238174.
   The function uses memoisation to compute all previously
   required values up to the largest argument seen. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cached_n  = 0;   /* largest n computed so far */
    static size_t capacity   = 0;   /* allocated size of cache (index 0..capacity-1) */

    /* Ensure cache is large enough to hold up to index n (1‑based) */
    if ((size_t)n > cached_n) {
        size_t new_cap = (size_t)n + 1;   /* +1 because we ignore index 0 */
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            capacity = new_cap;
        } else if (new_cap > capacity) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;  /* allocation failure: return 0 as a fallback */
            cache = tmp;
            capacity = new_cap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 8) {
                /* Initial terms (index 0 unused) */
                static const seq_t init_vals[9] = {
                    0ULL,            /* unused */
                    423ULL,          /* a(1) */
                    5321ULL,         /* a(2) */
                    69023ULL,        /* a(3) */
                    938601ULL,       /* a(4) */
                    12951743ULL,     /* a(5) */
                    180345769ULL,    /* a(6) */
                    2520461647ULL,   /* a(7) */
                    35289713177ULL   /* a(8) */
                };
                cache[i] = init_vals[i];
            } else {
                /* Recurrence (positive and negative contributions split) */
                seq_t pos = mul_small(cache[i-1], 22);
                pos = pos + mul_small(cache[i-4], 1473);
                pos = pos + mul_small(cache[i-7], 1514);

                seq_t neg = mul_small(cache[i-2], 111);
                neg = neg + mul_small(cache[i-3], 106);
                neg = neg + mul_small(cache[i-5], 2198);
                neg = neg + mul_small(cache[i-6], 121);
                neg = neg + mul_small(cache[i-8], 472);

                cache[i] = pos - neg;     /* pos > neg for valid terms */
            }
        }
        cached_n = n;   /* update memoisation bound */
    }

    return cache[n];   /* index is guaranteed to be valid now */
}
