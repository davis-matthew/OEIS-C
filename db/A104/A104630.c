/*
 * A104630  Expansion of x/(1-5*x+7*x^2-5*x^3+x^4).
 * https://oeis.org/A104630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 5*a(n-3) - a(n-4), a(0) = 0, a(1) =
 *            1, a(2) = 5, a(3) = 18
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 5;
    if (n == 3) return 18;

    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t cached = 0;     /* number of computed entries */
    static idx_t capacity = 0;   /* allocated size */

    /* Allocate and initialize cache on first use. */
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 5;
        cache[3] = 18;
        cached = 4;
    }

    /* Resize cache if necessary. */
    if (n + 1 > capacity) {
        idx_t newcap = capacity;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* Compute missing entries up to n. */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t value = 5 * cache[i - 1];
        value = value - (7 * cache[i - 2]);
        value = value + (5 * cache[i - 3]);
        value = value - cache[i - 4];
        cache[i] = value;
    }
    cached = n + 1;

    return cache[n];
}
