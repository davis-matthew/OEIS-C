/*
 * A159648  Numerator of Hermite(n, 10/19).
 * https://oeis.org/A159648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 20*a(n-1) - 722*(n-1)*a(n-2)
 * Initial values: a(0) = 1, a(1) = 20
 */
#include <stdlib.h>  /* malloc, realloc */
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;  /* number of initialized terms */

    /* allocate or enlarge cache if necessary */
    if (n >= cache_size) {
        size_t need = (size_t)n + 1;
        if (cache_cap < need) {
            size_t new_cap = need * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – terminate program */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)20;
            } else {
                seq_t term1 = (seq_t)20 * cache[i - 1];
                seq_t coeff = (seq_t)(722 * (i - 1));
                seq_t term2 = coeff * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
