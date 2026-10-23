/*
 * A116806  Number of permutations of length n which avoid the patterns 2314,
 *          4213, 4312.
 * https://oeis.org/A116806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 9*a(n-1) -
 *            31*a(n-2) + 52*a(n-3) * - 45*a(n-4) + 20*a(n-5) - 4*a(n-6) *
 *            with initial values * a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75,
 *            a(6)=262. * * This follows from the rational generating function
 *            * A(x) = -x(4x⁵-13x⁴+23x³-19x²+7x-1)/((2x-1)²(x-1)...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *p) */

static seq_t *seq_cache = NULL;  /* 1‑based cache, seq_cache[0] unused */
static idx_t   seq_max  = 0;      /* largest n that has been filled */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (n > seq_max) {
        /* Allocate a larger cache.  The old cache will be freed. */
        seq_t *new_cache = alloc_seq_array((size_t)(n + 1));
        if (seq_cache != NULL) {
            /* copy already computed values */
            for (idx_t i = 1; i <= seq_max; ++i) new_cache[i] = seq_cache[i];
            free_seq_array(seq_cache);
        }
        seq_cache = new_cache;

        /* Compute new values up to n */
        for (idx_t i = seq_max + 1; i <= n; ++i) {
            if (i == 1)      seq_cache[i] = 1;
            else if (i == 2) seq_cache[i] = 2;
            else if (i == 3) seq_cache[i] = 6;
            else if (i == 4) seq_cache[i] = 21;
            else if (i == 5) seq_cache[i] = 75;
            else if (i == 6) seq_cache[i] = 262;
            else
                seq_cache[i] = 9  * seq_cache[i-1]
                               -31 * seq_cache[i-2]
                               +52 * seq_cache[i-3]
                               -45 * seq_cache[i-4]
                               +20 * seq_cache[i-5]
                               -4  * seq_cache[i-6];
        }
        seq_max = n;
    }
    return seq_cache[n];
}
