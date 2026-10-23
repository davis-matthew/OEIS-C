/*
 * A026933  Self-convolution of array T given by A008288.
 * https://oeis.org/A026933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((5n‑3)a(n‑1) + (5n‑2)a(n‑2) – (n‑1)a(n‑3)) / n with
 *            a(0)=1, a(1)=2, a(2)=11 Recurrence uses integer division; the
 *            numerator is always divisible by the denominator
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static int cache_init = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache is initialized with the first three terms */
    if (!cache_init) {
        cache_cap = 3;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)11;
        cache_init = 1;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_cap)
        return cache[n];

    /* Grow the cache to hold a[n] */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;

        /* Compute each new term using the recurrence */
        for (size_t k = cache_cap; k < new_cap; ++k) {
            idx_t ik = (idx_t)k;              /* convert to idx_t for intermediate constants */

            if (sizeof(seq_t) <= 8) {         /* 64‑bit types: use 128‑bit intermediates */
                __int128 mul1 = (__int128)cache[k-1] * (__int128)(5*ik-3);
                __int128 mul2 = (__int128)cache[k-2] * (__int128)(5*ik-2);
                __int128 mul3 = (__int128)cache[k-3] * (__int128)(ik-1);
                __int128 num  = mul1 + mul2 - mul3;
                cache[k] = (seq_t)(num / (__int128)ik);
            } else {                          /* Big‑int build: rely on seq_t arithmetic */
                cache[k] =
                    ((seq_t)(5*ik-3) * cache[k-1]
                     + (seq_t)(5*ik-2) * cache[k-2]
                     - (seq_t)(ik-1) * cache[k-3]) / (seq_t)ik;
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
