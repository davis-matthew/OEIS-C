/*
 * A090192  Carlitz-Riordan q-Catalan numbers (recurrence version) for q = -1.
 * https://oeis.org/A090192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^{(n-1)/2} * Catalan((n-1)/2) for odd n; a(n)=0 for
 *            even n Catalan numbers are computed by the recurrence: C_0 = 1
 *            C_{k+1} = C_k * (4*k + 2) / (k + 2) (division is exact)
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 72, i128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if ((n & 1ULL) == 0)           /* even n > 0 */
        return (seq_t)0;

    /* n is odd; compute k = (n-1)/2 */
    idx_t k = (n - 1) / 2;

    /* Static storage for Catalan numbers */
    static seq_t *catalan = NULL;
    static size_t catalan_len = 0;   /* number of computed Catalan values */
    static size_t catalan_cap = 0;   /* allocated capacity */

    /* Ensure we have Catalan[0..k] available */
    if (k + 1 > catalan_len) {
        size_t required = (size_t)k + 1;

        /* Allocate or grow the array */
        if (required > catalan_cap) {
            size_t new_cap = catalan_cap ? catalan_cap * 2 : 1;
            if (new_cap < required)
                new_cap = required;
            seq_t *tmp = (seq_t *)realloc(catalan, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;   /* allocation failure */
            catalan = tmp;
            catalan_cap = new_cap;
        }

        /* Compute missing Catalan values */
        for (size_t i = catalan_len; i < required; ++i) {
            if (i == 0) {
                catalan[0] = (seq_t)1;
            } else {
                /* C_i = C_{i-1} * (4*(i-1)+2) / (i+1)  (exact division) */
                seq_t numer = catalan[i - 1] * (seq_t)(4ULL * (i - 1) + 2ULL);
                catalan[i]  = numer / (seq_t)(i + 1);
            }
        }
        catalan_len = required;
    }

    /* a(2k+1) = (-1)^k * Catalan(k) */
    seq_t sign = ((k & 1ULL) == 0) ? (seq_t)1 : (seq_t)-1;
    return sign * catalan[k];
}
