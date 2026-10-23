/*
 * A145135  Expansion of x/((1 - x - x^4)*(1 - x)^6).
 * https://oeis.org/A145135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 47, u64 n <= 116, u128 n <= 254,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Name: Expansion of x/((1 - x - x^4)*(1 - x)^6)
   Offset: 0
   Recurrence (for n ≥ 10):
     a(n) = 7*a(n-1) – 21*a(n-2) + 35*a(n-3) – 34*a(n-4) + 15*a(n-5) + 8*a(n-6) – 19*a(n-7) + 15*a(n-8) – 6*a(n-9) + a(n-10)
   Initial values:
     a(0)=0, a(1)=1, a(2)=7, a(3)=28, a(4)=84,
     a(5)=211, a(6)=470, a(7)=960, a(8)=1836, a(9)=3334
*/

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity (number of seq_t elements) */
    static idx_t  computed   = 9;   /* largest index already computed in cache */

    /* Initialise cache and base values on first use */
    if (cache == NULL) {
        if (cache_cap < 10) {
            /* Ensure room for the first 10 terms (index 0..9) */
            ensure_capacity(10, &cache_cap, (void **)&cache, sizeof(seq_t));
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)28;
        cache[4] = (seq_t)84;
        cache[5] = (seq_t)211;
        cache[6] = (seq_t)470;
        cache[7] = (seq_t)960;
        cache[8] = (seq_t)1836;
        cache[9] = (seq_t)3334;
        computed = 9;   /* highest computed index */
    }

    /* Return if already computed */
    if (n <= computed) {
        return cache[n];
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)(n + 1) > cache_cap) {
        ensure_capacity(n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Separate positive and negative contributions to avoid unsigned underflow */
        seq_t pos = cache[i-1] * (seq_t)7
                  + cache[i-3] * (seq_t)35
                  + cache[i-5] * (seq_t)15
                  + cache[i-6] * (seq_t)8
                  + cache[i-8] * (seq_t)15
                  + cache[i-10];
        seq_t neg = cache[i-2] * (seq_t)21
                  + cache[i-4] * (seq_t)34
                  + cache[i-7] * (seq_t)19
                  + cache[i-9] * (seq_t)6;
        cache[i] = pos - neg;   /* pos >= neg by construction */
    }

    computed = n;   /* update the highest computed index */
    return cache[n];
}
