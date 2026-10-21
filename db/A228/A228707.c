/*
 * A228707  G.f.:
 *          (1-3*x+5*x^2-5*x^3+5*x^4-5*x^5+5*x^6-3*x^7+x^8)/((1-x)^4*(1+x^4)*(1+x^2)^2).
 * https://oeis.org/A228707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   (1-x)^4(1+x^2)^2(1+x^4) * A(x) = 1 - 3x + 5x^2 - 5x^3 + 5x^4 - 5x^5 + 5x^6 - 3x^7 + x^8.
   Hence for n ≥ 12:
   a(n) = 4a(n-1) - 8a(n-2) + 12a(n-3) - 15a(n-4) + 16a(n-5) - 16a(n-6)
            + 16a(n-7) - 15a(n-8) + 12a(n-9) - 8a(n-10) + 4a(n-11) - a(n-12).
   Initial values:
   a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=6, a(5)=8, a(6)=10, a(7)=16,
   a(8)=24, a(9)=29, a(10)=35, a(11)=47. */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[12] = {
    1, 1, 1, 3, 6, 8, 10, 16, 24, 29, 35, 47
};

static seq_t *cache      = NULL;   /* pointers to cached values */
static idx_t cache_size = 0;       /* allocated array size */
static idx_t upto       = 11;      /* last computed index (initially 11) */

seq_t a(idx_t n)
{
    /*  Return known initial values directly. */
    if (n < 12)
        return init_vals[n];

    /*  Allocate or grow the cache if necessary. */
    if (!cache) {
        cache_size = n + 1;
        cache = seq_alloc(cache_size);
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
    } else if (cache_size <= n) {
        cache = seq_realloc(cache, cache_size, n + 1);
        cache_size = n + 1;
    }

    /*  Compute missing terms up to n.  */
    for (idx_t i = upto + 1; i <= n; ++i) {
        /*  Positive coefficients: 4,12,16,16,12,4  */
        seq_t pos =
              cache[i-1] * (seq_t)(4)      +
              cache[i-3] * (seq_t)(12)     +
              cache[i-5] * (seq_t)(16)     +
              cache[i-7] * (seq_t)(16)     +
              cache[i-9] * (seq_t)(12)     +
              cache[i-11] * (seq_t)(4);
        /*  Negative coefficients: 8,15,16,15,8,1  */
        seq_t neg =
              cache[i-2] * (seq_t)(8)      +
              cache[i-4] * (seq_t)(15)     +
              cache[i-6] * (seq_t)(16)     +
              cache[i-8] * (seq_t)(15)     +
              cache[i-10] * (seq_t)(8)     +
              cache[i-12] * (seq_t)(1);
        cache[i] = pos - neg;   /* all intermediate values stay non‑negative */
    }
    upto = n;
    return cache[n];
}
