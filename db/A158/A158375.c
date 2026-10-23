/*
 * A158375  Let p(x) = 1063*x + 867*x^2 + 676*x^3 + 322*x^4 + 124*x^5 + 36*x^6
 *          + 7*x^7 + x^8, expansion of the reciprocal polynomial of p(x).
 * https://oeis.org/A158375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -{7*a(n-1) + 36*a(n-2) + 124*a(n-3) + 322*a(n-4) +
 *            676*a(n-5) + 867*a(n-6) + 1063*a(n-7)}, with a(0)=1, a(1)=-7,
 *            a(2)=13, a(3)=37, a(4)=-181, a(5)=-99, a(6)=2300
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* recurrence coefficients (a[n-1] … a[n-7]) */
    static const seq_t coeff[7] = {7, 36, 124, 322, 676, 867, 1063};

    /* initial terms */
    static const seq_t init[7] = {1, -7, 13, 37, -181, -99, 2300};

    /* dynamic cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t  size  = 0;   /* number of terms currently stored */
    static idx_t  cap   = 0;   /* current allocated capacity */

    /* enlarge cache if necessary */
    if (cap <= n) {
        idx_t newcap = cap ? cap : 1;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* fill cache up to index n */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            seq_t sum = 0;
            for (int j = 0; j < 7; ++j) {
                sum += (seq_t)coeff[j] * cache[i - 1 - j];
            }
            cache[i] = -sum;
        }
    }
    size = n + 1;   /* update stored size */
    return cache[n];
}
