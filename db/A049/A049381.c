/*
 * A049381  Expansion of (1-25*x)^(-3/5).
 * https://oeis.org/A049381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*(5*n - 2)/n) * a(n-1); a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cap        = 0;
    static idx_t computed_max = 0;

    /* first call: allocate cache and set a(0) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;
        computed_max = 0;
    }

    /* enlarge cache if needed */
    if ((idx_t)(cap - 1) < n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n (if not already computed) */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t prev   = cache[i - 1];
            seq_t denom  = (seq_t)i;
            seq_t num    = (seq_t)5 * (((seq_t)5 * i) - 2);

            /* cancel common factors between denom and prev */
            seq_t g1      = gcd(prev, denom);
            seq_t prev_red  = prev / g1;
            seq_t denom_red = denom / g1;

            /* cancel remaining common factors between denom_red and num */
            seq_t g2      = gcd(num, denom_red);
            seq_t num_red  = num / g2;
            denom_red = denom_red / g2;   /* should equal 1 */

            /* compute new value: canceling avoids intermediate overflow */
            seq_t val = prev_red * num_red;
            cache[i] = val;
        }
        computed_max = n;
    }

    return cache[n];
}
