/*
 * A211550  Number of (n+1)X(n+1) -9..9 symmetric matrices with every 2X2
 *          subblock having sum zero and three distinct values.
 * https://oeis.org/A211550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   order‑17 linear recurrence with initial values a(1) = 150, a(2)
 *            = 330, …, a(17) = 3 211 388 a(n) = 5*a(n-1) + 6*a(n-2)
 *            -66*a(n-3) +32*a(n-4) +343*a(n-5) -396*a(n-6) -874*a(n-7)
 *            +1475*a(n-8) +1066*a(n-9) -2702*a(n-10) -373*a(n-11)
 *            +2577*a(n-12) -367*a(n-13) -1195*a(n-14) +320*a(n-15)
 *            +210*a(n-16)...
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 27, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort, abs */

seq_t a(idx_t n)
{
    /* recurrence coefficients (a(n-1) … a(n-17)) */
    static const int coeff[17] = {
        5, 6, -66, 32, 343, -396, -874, 1475, 1066,
        -2702, -373, 2577, -367, -1195, 320, 210, -60
    };

    /* initial 17 terms (1‑based, index 0 unused) */
    static const seq_t init[18] = {
        0ULL,          /* unused placeholder for 0 */
        150ULL, 330ULL, 640ULL, 1190ULL, 2172ULL,
        3922ULL, 7052ULL, 12734ULL, 22944ULL, 41816ULL,
        76046ULL, 140318ULL, 258302ULL, 482748ULL,
        900018ULL, 1702150ULL, 3211388ULL
    };

    /* cache for computed values; cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* largest n for which cache[n] is valid */

    if (n <= 17U)
        return init[n];

    /* grow cache if needed */
    if ((idx_t)cache_len < n) {
        size_t new_len = (size_t)(n + 1ULL);          /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                                 /* allocation failure */
        cache = tmp;

        /* initialise base case entries */
        if (cache_len == 0U) {
            for (idx_t i = 1U; i <= 17U; ++i)
                cache[i] = init[i];
            cache_len = 17U;
        }

        /* compute missing values using recurrence */
        for (idx_t i = cache_len + 1U; i <= n; ++i) {
            seq_t sum = 0;
            for (int k = 0; k < 17; ++k) {
                idx_t idx = i - (k + 1U);
                seq_t term = cache[idx] * (seq_t)abs(coeff[k]);
                sum += (coeff[k] >= 0) ? term : -term;
            }
            cache[i] = sum;
        }
        cache_len = n;
    }

    return cache[n];
}
