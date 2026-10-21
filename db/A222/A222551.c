/*
 * A222551  Number of (n+4)X1 arrays of occupancy after each element moves up
 *          to +-4 places but not 0.
 * https://oeis.org/A222551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 121, a(2) = 427, a(3) = 1562, a(4) = 5761, a(5) = 21239,
 *            a(6) = 78245, a(7) = 287858, a(8) = 1059674, a(9) = 3901868,
 *            a(10) = 14366833, a(11) = 52894813, a(12) = 194725954, a(13) =
 *            716822517, a(n) = 9*a(n-1) -28*a(n-2) +35*a(n-3) -15*a(n-4)
 *            +12*a(n-5) -96*a(n-6) +274*a(n-7) -265*a(n-8) +24...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   ensure_capacity, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;   /* largest n that has been computed (1‑based) */

static const seq_t baseVals[13] = {
    121ULL, 427ULL, 1562ULL, 5761ULL, 21239ULL, 78245ULL,
    287858ULL, 1059674ULL, 3901868ULL, 14366833ULL,
    52894813ULL, 194725954ULL, 716822517ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the offset 0 */

    /* extend cache up to n if needed */
    if (n > cache_size) {
        /* enlarge storage */
        if (n > cache_cap) {
            ensure_capacity(n, &cache_cap, (void **)&cache, sizeof(seq_t));
        }

        /* compute entries from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t value;
            if (i <= 13) {
                value = baseVals[i - 1];
            } else {
                /* recurrence of order 13 */
                static const int coeffs[13] = {
                    9, -28, 35, -15, 12, -96, 274, -265,
                    24, -7, 29, 3, -1
                };
                seq_t sum_pos = 0;
                seq_t sum_neg = 0;
                for (int k = 1; k <= 13; ++k) {
                    seq_t prev = cache[i - k - 1];   /* a(i-k) */
                    int c = coeffs[k - 1];
                    seq_t term = mul_small(prev, (unsigned int)(c > 0 ? c : -c));
                    if (c > 0)
                        sum_pos += term;
                    else
                        sum_neg += term;
                }
                value = sum_pos;
                value -= sum_neg;                  /* subtraction is safe (value>=0) */
            }
            cache[i - 1] = value;
        }
        cache_size = n;
    }
    return cache[n - 1];
}
