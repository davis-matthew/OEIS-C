/*
 * A169259  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A169259: Number of reduced words of length n in the Coxeter
    group on 6 generators S_i with (S_i)^2 = (S_i S_j)^29 = I.
    The sequence satisfies the linear recurrence
        a(0) = 1
        a(n) = 4 * sum_{k=1}^{28} a(n-k) - 10 * a(n-29) + p(n)   (n ≥ 1)
    where the polynomial p(n) comes from the numerator of the
    generating function:
        p(0) = p(29) = 1,
        p(1) … p(28) = 2,
        p(n) = 0 for n > 29.
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* array of pre‑computed values  */
    static idx_t cache_size = 0;    /* number of elements stored */

    /* enlarge cache if needed */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* cannot allocate */

        /* initialise the first element if cache empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache_size = 1;
        }

        /* compute values from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;

            /* sum of the last 28 terms a(i-1), ..., a(i-28) */
            for (idx_t k = 1; k <= 28; ++k) {
                if (i >= k) sum += cache[i - k];
            }

            seq_t term = (seq_t)4 * sum;   /* 4 * sum */

            if (i >= 29)                        /* -10 * a(i-29) */
                term -= (seq_t)10 * cache[i - 29];

            /* add polynomial p(i) */
            if (i == 0 || i == 29)
                term += (seq_t)1;
            else if (i <= 28)
                term += (seq_t)2;   /* for 1 ≤ i ≤ 28 */

            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
