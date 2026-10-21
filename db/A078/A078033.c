/*
 * A078033  Expansion of (1-x) / (1+x^2+2*x^3).
 * https://oeis.org/A078033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -1, a(2) = -1 a(n) = -a(n-2) - 2*a(n-3) (for n
 *            > 2) This is the expansion of (1‑x)/(1+x²+2x³)
 * Range:     i8 n <= 14, i16 n <= 32, i32 n <= 63, i64 n <= 128,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *measure_cache = NULL;
static idx_t cache_size = 0;

/* Return the nth term of OEIS A078033 */
seq_t a(idx_t n)
{
    /* Ensure cache is allocated up to n+1 */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(measure_cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – fall back to minimal error handling */
            abort();
        }
        measure_cache = tmp;
        /* Initialize new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                measure_cache[i] = (seq_t)1;
            } else if (i == 1 || i == 2) {
                measure_cache[i] = (seq_t)-1;
            } else {
                measure_cache[i] = -measure_cache[i - 2] - ((seq_t)2 * measure_cache[i - 3]);
            }
        }
        cache_size = new_size;
    }
    return measure_cache[n];
}
