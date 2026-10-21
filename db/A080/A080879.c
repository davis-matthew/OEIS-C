/*
 * A080879  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=1, a(2)=6.
 * https://oeis.org/A080879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=6, a(3)=7; for n≥4, a(n)=8*a(n-2)-4*a(n-4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define CACHE_SIZE 20000

static seq_t cache[CACHE_SIZE];
static idx_t max_computed = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* initialise first four terms once */
    if (!initialized) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 7;
        max_computed = 3;
        initialized = true;
    }

    /* if value already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* extend cache up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 8 * cache[i - 2] - 4 * cache[i - 4];
    }
    max_computed = n;
    return cache[n];
}
