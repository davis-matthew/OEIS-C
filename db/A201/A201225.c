/*
 * A201225  Values x for infinite sequence x^3-y^2 = d with decreasing
 *          coefficient r=sqrt(x)/d which tend to 1/(1350*sqrt(5))or infinity
 *          family of solutions Mordell curve with extension sqrt(5).
 * https://oeis.org/A201225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies linear recurrence a(n) = 341*a(n-1) - 6138*a(n-2)
 *            + 6138*a(n-3) - 341*a(n-4) + a(n-5), with initial values: a(1) =
 *            6100 a(2) = 2305180 a(3) = 748476100 a(4) = 241118603980 a(5) =
 *            77641444770100
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef MAX_CACHE_SIZE
#define MAX_CACHE_SIZE 20001   /* safe for n up to ~10000 */
#endif

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t current_max = 0;
    static seq_t cache[MAX_CACHE_SIZE];

    if (!initialized) {
        /* initialize first five terms */
        cache[1] = 6100ull;
        cache[2] = 2305180ull;
        cache[3] = 748476100ull;
        cache[4] = 241118603980ull;
        cache[5] = 77641444770100ull;
        current_max = 5;
        initialized = true;
    }

    if (n <= current_max)
        return cache[n];

    /* extend cache if necessary */
    if (n >= MAX_CACHE_SIZE) {
        /* in principle, resize logic could be added here */
        /* but per problem constraints this case should not occur */
        return 0;
    }

    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)341 * (cache[i-1] - cache[i-4]);
        seq_t term2 = (seq_t)6138 * (cache[i-3] - cache[i-2]);
        cache[i] = cache[i-5] + term1 + term2;
    }
    current_max = n;
    return cache[n];
}
