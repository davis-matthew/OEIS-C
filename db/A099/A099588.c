/*
 * A099588  Coefficient of x^2 in (1+x)^n mod 1+x^4.
 * https://oeis.org/A099588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k>=0} (-1)^k * binomial(n, 4k+2) Recurrence: a(n) =
 *            4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-4) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=3
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 36, i64 n <= 73, i128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#endif
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(4);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 3;
    }
    if (n >= cache.size()) {
        cache.resize(n + 1);
        for (idx_t i = 4; i <= n; ++i) {
            cache[i] = (4 * cache[i-1]
                        - 6 * cache[i-2]
                        + 4 * cache[i-3]
                        - 2 * cache[i-4]);
        }
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity              */
    static idx_t last  = 0;   /* highest index that has a value*/

    /* Allocate initial storage and initialise base cases. */
    if (cache == NULL) {
        cap = (n > 15) ? (n + 1) : 16;   /* start with a reasonable size     */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 3;
        last = 3;
    }
    /* Grow storage if needed. */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – return 0 */
            return 0;
        }
        cache = tmp;
        cap = newcap;
    }
    /* Compute missing terms up to n. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = (4 * cache[i-1]
                        - 6 * cache[i-2]
                        + 4 * cache[i-3]
                        - 2 * cache[i-4]);
        }
        last = n;
    }
    return cache[n];
#endif
}
