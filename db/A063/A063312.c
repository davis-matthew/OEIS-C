/*
 * A063312  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          39 ).
 * https://oeis.org/A063312
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence derived from the given generating function:
 *  (x-1)^2*(x+1)^2*(x^2 - x + 1)*(x^2 + 1) * A(x) =
 *  x^2*(-1 + 30x + 43x^2 + 41x^3 + 73x^4 + 53x^5 + 41x^6 + 43x^7 + 8x^8 + x^9)
 *
 * The recurrence for n ≥ 0 is
 *   a(n) = coeff_N(n) + a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8)
 *   where coeff_N(n) is non‑zero only for 2 ≤ n ≤ 11:
 *
 *     coeff_N(2)= -1
 *     coeff_N(3)= 30
 *     coeff_N(4)= 43
 *     coeff_N(5)= 41
 *     coeff_N(6)= 73
 *     coeff_N(7)= 53
 *     coeff_N(8)= 41
 *     coeff_N(9)= 43
 *     coeff_N(10)=8
 *     coeff_N(11)=1
 *
 * For n < 2 the sequence is defined to be 0 (values below the offset are irrelevant).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* values below the offset are irrelevant */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_last = 0;          /* index of highest value cached */
    static size_t cache_len  = 0;          /* current allocated length */

    /* helper array of coeff_N for 0…11 (0 otherwise) */
    static const seq_t coeff_N[12] = {
        0, 0, -1, 30, 43, 41, 73, 53, 41, 43, 8, 1
    };

    /* initialise cache if first call or enlarge if needed */
    if (cache == NULL || cache_len <= n) {
        size_t new_len = (size_t)(n) + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed – return 0 as safe default */
        cache = tmp;
        if (cache_len == 0) {
            /* initialise a[0] = a[1] = 0 */
            cache[0] = 0;
            if (new_len > 1) cache[1] = 0;
        }
        /* if we grew, set the new elements to 0 (needed for convenience) */
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* compute new terms up to n if necessary */
    for (idx_t k = cached_last + 1; k <= n; ++k) {
        seq_t val = 0;
        /* coeff_N(k) is defined only for k <= 11 */
        if (k < 12) val += coeff_N[k];

        if (k >= 1) val += cache[k - 1];
        if (k >= 3) val -= cache[k - 3];
        if (k >= 4) val += 2 * cache[k - 4];
        if (k >= 5) val -= cache[k - 5];
        if (k >= 7) val += cache[k - 7];
        if (k >= 8) val -= cache[k - 8];

        cache[k] = val;
    }
    cached_last = n;

    return cache[n];
}
