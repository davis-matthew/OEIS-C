/*
 * A187365  Trisection of A000984 (central binomial coefficients):
 *          binomial(2(3n+2),3n+2)/3!, n>=0.
 * https://oeis.org/A187365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=a(n-1)*8*(2*n+1)*(6*n+1)*(6*n-1)/(n*(3*n+1)*(3*n+2)) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* enlarge cache when needed */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();   /* allocation failed */
        cache = (seq_t*)p;
        cache_cap = new_cap;
    }

    /* compute values up to n if not already */
    if (cache_len <= (size_t)n) {
        if (cache_len == 0) {      /* initialise base case */
            cache[0] = (seq_t)1;
            cache_len = 1;
        }

        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            idx_t ii = (idx_t)i;

            seq_t prev = cache[i - 1];

            /* numerator and denominator factors of the recurrence */
            seq_t num[4];
            seq_t den[3];

            num[0] = (seq_t)8;
            num[1] = (seq_t)(2 * ii + 1);
            num[2] = (seq_t)(6 * ii + 1);
            num[3] = (seq_t)(6 * ii - 1);

            den[0] = (seq_t)ii;
            den[1] = (seq_t)(3 * ii + 1);
            den[2] = (seq_t)(3 * ii + 2);

            /* cancel common factors between prev and denominator */
            for (int d = 0; d < 3; ++d) {
                seq_t g = gcd(prev, den[d]);
                if (g > 1) {
                    prev /= g;
                    den[d] /= g;
                }
            }

            /* cancel remaining denominator with numerator factors */
            for (int d = 0; d < 3; ++d) {
                if (den[d] == (seq_t)1)
                    continue;
                for (int j = 0; j < 4; ++j) {
                    seq_t g = gcd(num[j], den[d]);
                    if (g > 1) {
                        num[j] /= g;
                        den[d] /= g;
                        if (den[d] == (seq_t)1)
                            break;
                    }
                }
            }

            /* after cancellation every den[d] should be 1 */
            seq_t ans = prev;
            for (int j = 0; j < 4; ++j)
                ans *= num[j];

            cache[i] = ans;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
