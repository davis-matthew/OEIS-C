/*
 * A170840  G.f.: Product_{k>=2} (1 + x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Product_{k>=2} (1 + x^{2^k-1} +
 *            3x^{2^k})
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed coefficients up to the largest n requested */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;           /* maximum index currently cached */

    /* If cache is empty, initialise it for n=0 */
    if (cache == NULL) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        cache[0] = 1;
        cached_max = 0;
    }

    /* If requested n is larger than cached, recompute up to n */
    if (n > cached_max) {
        idx_t new_max = n;
        size_t new_len = (size_t)new_max + 1;

        /* Allocate new array and initialise to zero */
        seq_t *new_cache = (seq_t*)calloc(new_len, sizeof(seq_t));
        if (!new_cache) return 0;                 /* allocation failed; return 0 */
        new_cache[0] = 1;

        /* Compute coefficients up to new_max using dynamic programming */
        for (idx_t power = 1ULL << 2, k = 2; 
             (power - 1 <= new_max) || (power <= new_max); 
             ++k, power <<= 1) {

            idx_t exp1 = power - 1;   /* exponent 2^k - 1 */
            idx_t exp2 = power;       /* exponent 2^k     */

            /* iterate backwards to avoid counting a factor more than once */
            for (size_t i = (size_t)new_max; i != (size_t)-1; --i) {
                seq_t val = new_cache[i];
                if (!val) continue;

                if (i + exp1 <= new_max) {
                    new_cache[i + exp1] += val;
                }
                if (i + exp2 <= new_max) {
                    seq_t val3 = val * 3;
                    new_cache[i + exp2] += val3;
                }
            }
        }

        /* Free old cache and replace */
        free(cache);
        cache = new_cache;
        cached_max = new_max;
    }

    return cache[n];
}
