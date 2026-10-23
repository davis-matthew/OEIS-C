/*
 * A159954  Numerator of Hermite(n, 5/24).
 * https://oeis.org/A159954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5*a(n-1) - 288*(n-1)*a(n-2). a(0)=1, a(1)=5
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t allocated = 0;   /* number of entries allocated  */
    static idx_t computed = 0;    /* next index to compute (0 if all up to 0 computed) */

    /* ensure capacity */
    if (n + 1 > allocated) {
        idx_t new_cap = n + 1;
        seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (allocated > 0) {
            memcpy(new_mem, cache, allocated * sizeof(seq_t));
            free(cache);
        }
        cache = new_mem;
        allocated = new_cap;
    }

    /* compute values up to n if necessary */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)5;
        } else {
            seq_t term1 = (seq_t)5 * cache[i - 1];
            seq_t factor = (seq_t)(i - 1) * (seq_t)288;
            seq_t term2 = factor * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }
    computed = n + 1;  /* all values up to n are now computed */

    return cache[n];
}
