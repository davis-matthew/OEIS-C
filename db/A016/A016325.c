/*
 * A016325  Expansion of 1/((1 - 2*x)*(1 - 10*x)*(1 - 11*x)).
 * https://oeis.org/A016325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 110*a(n-2) + 2^n (n ≥ 2), a(0)=1, a(1)=23
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;        /* dynamic array of computed terms */
static size_t cache_cap = 0;        /* current capacity of the array       */
static idx_t  cache_len = 0;        /* number of terms currently stored    */

static seq_t pow2;                 /* 2^i for the current i               */

/* Make sure cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* First‑time initialisation */
    if (cache == NULL) {
        /* Allocate room for at least the first two terms */
        ensure_cache(1);
        cache[0] = (seq_t)1;          /* a(0)  */
        cache[1] = (seq_t)23;         /* a(1)  */
        cache_len = 2;

        /* Initialise pow2 to 2^cache_len (i.e., 2^2 = 4) */
        pow2 = (seq_t)1;
        for (idx_t k = 0; k < cache_len; ++k)
            pow2 = mul_small(pow2, 2);
    }

    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* Extend cache to hold indices up to n (inclusive) */
    ensure_cache(n);

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 21*a(i-1) - 110*a(i-2) + 2^i  */
        seq_t prod1 = mul_small(cache[i - 1], 21);
        seq_t prod2 = mul_small(cache[i - 2], 110);
        seq_t val   = prod1 - prod2 + pow2;   /* pow2 is 2^i */

        cache[i] = val;

        /* Advance pow2 to 2^(i+1) for the next iteration */
        pow2 = mul_small(pow2, 2);
    }

    cache_len = n + 1;   /* All indices up to n are now stored */
    return cache[n];
}
