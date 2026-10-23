/*
 * A041139  Denominators of continued fraction convergents to sqrt(78).
 * https://oeis.org/A041139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 106*a(n-4) - a(n-8) Initial terms (OEIS A041139): a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=6, a(4)=101, a(5)=107, a(6)=529, a(7)=636
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static const seq_t init_vals[8] = {1, 1, 5, 6, 101, 107, 529, 636};

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp;

        if (cache == NULL) {
            tmp = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        }
        if (tmp == NULL) {
            /* Allocation failed – return 0 (undefined behaviour for caller) */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 8) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = 106 * cache[i - 4] - cache[i - 8];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
