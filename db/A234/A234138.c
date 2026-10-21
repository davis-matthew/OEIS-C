/*
 * A234138  Number of (n+1) X (6+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 9.
 * https://oeis.org/A234138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234138 (n ≥ 1)
   Recurrence:
   a(n) = 3*a(n-1) + 8*a(n-2) - 30*a(n-3) + 4*a(n-4) + 48*a(n-5) - 32*a(n-6)
   Initial terms (needed for the 6‑term recurrence):
   a(1)=3584, a(2)=4040, a(3)=4808, a(4)=6440, a(5)=9512, a(6)=16424 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache       = NULL;   /* dynamic array of already computed terms */
static size_t  cache_cap  = 0;       /* allocated capacity (number of terms) */
static size_t  cache_len  = 0;       /* number of terms currently stored */

/* Initial six values – the recurrence requires them as seeds. */
static const seq_t init[6] = {
    (seq_t)3584,
    (seq_t)4040,
    (seq_t)4808,
    (seq_t)6440,
    (seq_t)9512,
    (seq_t)16424
};

seq_t a(idx_t n)
{
    if (n < 1) /* offset is 1 – undefined for n < 1 */
        return (seq_t)0;

    /* Grow the cache if it is too small. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Return already computed value if available. */
    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* Compute terms from cache_len+1 up to n (1‑based indices). */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 6) {                     /* seed values */
            cache[(size_t)i-1] = init[(size_t)i-1];
        } else {
            /* Use the recurrence. */
            seq_t term = mul_small(cache[(size_t)i-2], 3);      /* 3*a(i-1) */
            term += mul_small(cache[(size_t)i-3], 8);          /* +8*a(i-2) */

            /* Subtract parts of the recurrence. */
            term -= mul_small(cache[(size_t)i-4], 30);         /* -30*a(i-3) */

            /* Add the remaining terms. */
            term += mul_small(cache[(size_t)i-5], 4);          /* +4*a(i-4) */
            term += mul_small(cache[(size_t)i-6], 48);         /* +48*a(i-5) */
            term -= mul_small(cache[(size_t)i-7], 32);         /* -32*a(i-6) */

            cache[(size_t)i-1] = term;
        }
    }

    cache_len = (size_t)n;
    return cache[(size_t)n-1];
}
