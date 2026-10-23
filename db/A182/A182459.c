/*
 * A182459  a(n) is the number of initial persons such that the n-th person
 *          survives in the duck-duck-goose game.
 * https://oeis.org/A182459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A081615(n)-1 Recurrence: a(1)=1; a(n+1) = (3^m*(3*a(n)+2)
 *            - 2^m)/2^(m+1), where m = A007814(3*a(n)+2) (the exponent of 2
 *            dividing 3*a(n)+2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_n = 0;

static void grow_cache(idx_t need) {
    if (need < cache_size) return;
    size_t new_size = cache_size ? cache_size : 16;
    while (new_size <= need) new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort(); /* out‑of‑memory */
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n<1, but safe */
    /* ensure cache[1] is initialized */
    if (max_n == 0) {
        grow_cache(1);
        cache[1] = (seq_t)1;
        max_n = 1;
    }
    /* extend cache up to n */
    if (n > max_n) {
        grow_cache(n);
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* compute a(i) from a(i-1) */
            seq_t prev = cache[i - 1];
            seq_t s = prev * (seq_t)3 + (seq_t)2;      /* 3*a + 2 */

            /* find m = v2(s) */
            idx_t m = 0;
            seq_t temp = s;
            const seq_t ONE = (seq_t)1;
            while ((temp & ONE) == 0) {
                temp >>= 1;
                ++m;
            }

            /* compute 3^m */
            seq_t pow3 = ONE;
            for (idx_t j = 0; j < m; ++j) pow3 *= (seq_t)3;

            /* compute 2^m and 2^(m+1) */
            seq_t pow2m = ONE;
            for (idx_t j = 0; j < m; ++j) pow2m *= (seq_t)2;
            seq_t pow2m1 = pow2m * (seq_t)2;  /* 2^(m+1) */

            /* numerator = 3^m * s - 2^m */
            seq_t numerator = pow3 * s - pow2m;

            /* a(i) = numerator / 2^(m+1) */
            cache[i] = numerator / pow2m1;
        }
        max_n = n;
    }
    return cache[n];
}
