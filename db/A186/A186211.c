/*
 * A186211  Number of (n+1)X2 0..2 arrays with every 2X2 subblock determinant
 *          equal to some horizontal or vertical neighbor 2X2 subblock
 *          determinant.
 * https://oeis.org/A186211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+a(n-2)-48*a(n-3)+77*a(n-4)+28*a(n-5)
 *            -238*a(n-6)+230*a(n-7)-67*a(n-8)+170*a(n-9)+100*a(n-10)
 *            -432*a(n-11)+256*a(n-12)-532*a(n-13)+296*a(n-14)+144*a(n-15)
 *            -320*a(n-16)+512*a(n-17) for n>18 Initial values: a(1)=0,
 *            a(2)=175, a(3)=609, a(4)=3593, a(5)=15055, a(6)=76263,
 *            a(7)=353985, a...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   free_seq_array, mul_small, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

static const seq_t init[18] = {
    (seq_t)0ULL, (seq_t)175ULL, (seq_t)609ULL, (seq_t)3593ULL, (seq_t)15055ULL,
    (seq_t)76263ULL, (seq_t)353985ULL, (seq_t)1729733ULL, (seq_t)8225815ULL,
    (seq_t)39664199ULL, (seq_t)189800057ULL, (seq_t)911926009ULL,
    (seq_t)4372168391ULL, (seq_t)20986330867ULL, (seq_t)100672272185ULL,
    (seq_t)483086490365ULL, (seq_t)2317736112319ULL, (seq_t)11120991460315ULL
};

static const int coeff[17] = {
    6, 1, -48, 77, 28, -238, 230, -67, 170, 100,
    -432, 256, -532, 296, 144, -320, 512
};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed_high = 0;   /* number of terms already computed (max index+1) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* not defined, safe return */

    /* Ensure the cache has enough space for indices up to n-1 */
    if (n > cache_cap) {
        size_t new_cap = n > 64 ? (size_t)n : 64;
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (!new_cache) return (seq_t)0; /* allocation failure */
        /* copy existing values */
        if (cache) {
            for (size_t i = 0; i < cache_cap && i < new_cap; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_cap = (idx_t)new_cap;

        /* initialise first 18 terms if not yet done */
        if (computed_high == 0) {
            size_t init_len = 18;
            if (init_len > cache_cap) init_len = cache_cap;
            for (size_t i = 0; i < init_len; ++i)
                cache[i] = init[i];
            computed_high = (idx_t)init_len;
        }
    }

    /* Compute missing terms up to n-1 */
    if (n > computed_high) {
        idx_t start = computed_high;
        if (start < 18 && n > 18) start = 18;  /* skip pre-filled part */
        for (idx_t idx = start; idx < n; ++idx) {
            /* idx corresponds to a(idx+1) */
            seq_t pos = (seq_t)0;
            seq_t neg = (seq_t)0;
            for (int j = 0; j < 17; ++j) {
                idx_t prev = idx - 1 - j;
                seq_t term = mul_small(cache[prev], (unsigned int)(coeff[j] < 0 ? -coeff[j] : coeff[j]));
                if (coeff[j] >= 0)
                    pos = pos + term;
                else
                    neg = neg + term;
            }
            cache[idx] = pos - neg;
        }
        computed_high = n;
    }

    return cache[n - 1];
}
