/*
 * A036771  Number of labeled rooted trees with a degree constraint:
 *          ((3*n)!/(6^n)) * binomial(3*n + 1, n).
 * https://oeis.org/A036771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n)!/(6^n)) * binomial(3*n + 1, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n+1)=a(n)*3*(3n+4)*(3n+1)*(3n+2)^2/(4*(2n+3)) */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity */
    static size_t count = 0;          /* number of values stored (next free idx) */

    /* Allocate initial cache if needed */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, graceful exit */
    }

    /* Grow cache if requested n exceeds current capacity */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = count; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;                     /* a(0) = 1 */
        } else {
            /* recurrence: a(i) derived from a(i-1) */
            idx_t k = i - 1;                     /* previous index */
            seq_t next = cache[i - 1];            /* start with a(i-1) */

            /* numerator and denominator factors (5 and 2 factors) */
            seq_t num[5];
            seq_t den[2];

            num[0] = 3;
            num[1] = (seq_t)(3 * k + 4);
            num[2] = (seq_t)(3 * k + 1);
            seq_t t = (seq_t)(3 * k + 2);
            num[3] = t;
            num[4] = t;

            den[0] = 4;
            den[1] = (seq_t)(2 * k + 3);

            /* Reduce common factors to keep intermediate values small */
            for (int a = 0; a < 5; ++a) {
                for (int d = 0; d < 2; ++d) {
                    seq_t g = gcd(num[a], den[d]);
                    if (g > 1) {
                        num[a] /= g;
                        den[d] /= g;
                    }
                }
            }

            /* Multiply remaining numerators */
            for (int a = 0; a < 5; ++a) {
                next *= num[a];
            }

            /* Divide by remaining denominators (exact division) */
            for (int d = 0; d < 2; ++d) {
                next /= den[d];
            }

            cache[i] = next;
        }
    }

    count = (size_t)n + 1;
    return cache[n];
}
