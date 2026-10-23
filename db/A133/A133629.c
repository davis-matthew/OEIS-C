/*
 * A133629  a(1)=1, a(n) = a(n-1) + (p-1)*p^(n/2-1) if n is even, otherwise
 *          a(n) = a(n-1) + p^((n-1)/2), where p=5.
 * https://oeis.org/A133629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A133629:  a(1)=1, a(n)=a(n-1)+(p-1)*p^(n/2-1) if n even,
 *              a(n)=a(n-1)+p^((n-1)/2) if n odd,  with p=5.
 * This implementation uses memoised iterative evaluation.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1)  for 0 <= i < cache_size   */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)  /* offset is 1; n must be >=1, but guard anyway */
        return 0;

    /* initialise base case */
    if (n == 1) {
        if (cache_size < 1) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t));
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        return cache[0];
    }

    /* grow cache if necessary */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)  /* allocation failed; fall back to zero */
            new_cache = cache;
        cache = new_cache;

        const uint64_t P = 5;        /* prime parameter, p */
        const seq_t Pm1 = (seq_t)(P - 1); /* p-1 = 4 */

        /* fill values from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t delta;
            if ((i & 1u) == 0) {          /* even i */
                idx_t exp = i / 2u - 1u;   /* p^(i/2-1) */
                seq_t pow = pow_uint(P, exp);
                delta = Pm1 * pow;        /* (p-1)*p^(i/2-1) */
            } else { /* odd i */
                idx_t exp = (i - 1u) / 2u; /* p^((i-1)/2) */
                delta = pow_uint(P, exp);
            }
            cache[i - 1] = cache[i - 2] + delta;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
