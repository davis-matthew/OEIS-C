/*
 * A099624  a(n) = Sum_{k=0..floor(n/2)} C(n-k,k+2)*3^(n-k-2)*(4/3)^k.
 * https://oeis.org/A099624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1)-23*a(n-2)+3*a(n-3)+36*a(n-4) for n≥4
   Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=9
   (Formula from OEIS A099624) */

#include <stdlib.h>

static const seq_t init_vals[4] = {0, 0, 1, 9};

seq_t a(idx_t n)
{
    /* Handle n<4 immediately */
    if (n < 4) return init_vals[n];

    /* Static cache for previous terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 3;   /* last index computed so far */

    /* Initial allocation */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t val = 9 * a1 + 3 * a3 + 36 * a4 - 23 * a2;
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
