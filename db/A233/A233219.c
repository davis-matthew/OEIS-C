/*
 * A233219  Number of 3Xn 0..5 arrays with no element x(i,j) adjacent to value
 *          5-x(i,j) horizontally or antidiagonally, top left element zero,
 *          and 1 appearing before 2 3 and 4, and 2 appearing before 3 in row
 *          major order.
 * https://oeis.org/A233219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 11, a(2) = 452, a(3) = 35446, a(4) = 3054973, a(5) =
 *            268289572, a(6) = 23644611625, a(7) = 2085188355031, for n > 7:
 *            a(n) = 111*a(n-1) − 2128*a(n-2) + 10532*a(n-3) − 17559*a(n-4) +
 *            9045*a(n-5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoized values, indices 0.. */
    static idx_t allocated = 0;     /* number of slots allocated */

    if (n == 0)
        return 0;                    /* undefined, but safe */

    /* Allocate enough space for indices 0..n */
    if (n + 1 > allocated) {
        size_t newlen = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!cache)                      /* allocation failure – unlikely */
            return 0;

        /* Base values for n = 1..7 */
        static const seq_t init[7] = {
            11ULL,
            452ULL,
            35446ULL,
            3054973ULL,
            268289572ULL,
            23644611625ULL,
            2085188355031ULL
        };

        /* Fill new entries */
        for (idx_t i = allocated; i < newlen; ++i) {
            if (i == 0) {                /* index 0 unused */
                continue;
            }
            if (i <= 7) {                /* initial terms */
                cache[i] = init[i - 1];
            } else {                     /* recurrence for i >= 8 */
                cache[i] = (seq_t)111 * cache[i - 1]
                          - (seq_t)2128 * cache[i - 2]
                          + (seq_t)10532 * cache[i - 3]
                          - (seq_t)17559 * cache[i - 4]
                          + (seq_t)9045 * cache[i - 5];
            }
        }
        allocated = newlen;
    }

    return cache[n];
}
