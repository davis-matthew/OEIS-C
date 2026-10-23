/*
 * A170273  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence derived from the given generating function:
 *
 *      a(0) = 1
 *      For n ≥ 1:
 *          a(n) = 22 * (a(n−1)+a(n−2)+…+a(n−40))
 *                 – 253 * a(n−41)
 *                 + c(n)
 *
 *      The constant term c(n) equals:
 *          c(0) = 1
 *          c(n) = 2   for 1 ≤ n ≤ 40
 *          c(41) = 1
 *          c(n) = 0   for n > 41
 *
 *  This implementation uses memoisation so that calls with arbitrary
 *  indices are answered in O(1) after the required terms have been
 *  computed once.  It works for any `seq_t` type that supports the
 *  required arithmetic operators.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t computed = 0;          /* number of values already computed
                                           (next index to compute) */
    static idx_t cache_sz = 0;          /* allocated size of the cache array */
    static seq_t *cache = NULL;         /* array of memoised values */

    /* Allocate or grow cache if necessary */
    if (n + 1 > cache_sz) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – return 0
                                           as a fallback */
        cache = tmp;
        cache_sz = (idx_t)new_sz;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            seq_t sum = 0;

            /* 22 * sum_{k=1}^{40} a(i-k) */
            for (idx_t k = 1; k <= 40 && i >= k; ++k) {
                sum += cache[i - k] * (seq_t)22;
            }

            /* – 253 * a(i-41) if applicable */
            if (i >= 41) {
                sum -= cache[i - 41] * (seq_t)253;
            }

            /* Add the constant coefficient c(i) */
            seq_t coeff = 0;
            if (i == 41) {
                coeff = 1;
            } else if (i <= 40) {
                coeff = 2;
            }
            /* Note: i==0 is handled above, coeff for i>41 remains 0 */

            sum += coeff;
            cache[i] = sum;
        }
    }

    computed = n + 1;
    return cache[n];
}
