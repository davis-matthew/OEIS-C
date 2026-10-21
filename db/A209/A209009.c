/*
 * A209009  Number of 5-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and first and second differences in -n..n.
 * https://oeis.org/A209009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) + a(n-5) - 2*a(n-6) +
 *            2*a(n-8) - a(n-9) + a(n-10) - 2*a(n-11) + 2*a(n-13) - a(n-14) -
 *            a(n-15) + 2*a(n-16) - 2*a(n-18) + a(n-19) (for n > 19), with
 *            initial values: a(1)=1, a(2)=5, a(3)=13, a(4)=31, a(5)=71,
 *            a(6)=137, a(7)=243, a(8)=399, a(9)=619, a(10)=927, a(11...
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 0;

    /* initial values for 1..19 */
    static const seq_t init[20] = {
        0, 1, 5, 13, 31, 71, 137, 243, 399, 619,
        927, 1329, 1857, 2525, 3355, 4385, 5627,
        7121, 8893, 10971
    };

    /* initialise cache on first call */
    if (!cache) {
        cap = 20;                          /* indices 0 .. 19 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        for (size_t i = 1; i <= 19; ++i) cache[i] = init[i];
        last = 19;
    }

    size_t idx = (size_t)n;

    /* grow the cache if necessary */
    if (idx >= cap) {
        size_t newCap = idx + 1;
        seq_t *newCache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!newCache) return 0;           /* allocation failure */
        cache = newCache;
        /* initialise new cells to 0 */
        for (size_t i = cap; i < newCap; ++i) cache[i] = 0;
        cap = newCap;
    }

    /* if already computed, return */
    if (idx <= last) return cache[idx];

    /* compute terms from last+1 up to n */
    for (size_t i = last + 1; i <= idx; ++i) {
        seq_t val = 0;
        val += 2 * cache[i - 1];
        val -= 2 * cache[i - 3];
        val += 1 * cache[i - 4];
        val += 1 * cache[i - 5];
        val -= 2 * cache[i - 6];
        val += 2 * cache[i - 8];
        val -= 1 * cache[i - 9];
        val += 1 * cache[i -10];
        val -= 2 * cache[i -11];
        val += 2 * cache[i -13];
        val -= 1 * cache[i -14];
        val -= 1 * cache[i -15];
        val += 2 * cache[i -16];
        val -= 2 * cache[i -18];
        val += 1 * cache[i -19];
        cache[i] = val;
    }
    last = idx;
    return cache[idx];
}
