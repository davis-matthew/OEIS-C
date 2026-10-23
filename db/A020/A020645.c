/*
 * A020645  Least positive integer k for which 4^n divides k!.
 * https://oeis.org/A020645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that 4^n divides k!
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t v2_of(idx_t x)
{
    seq_t c = 0;
    while ((x & 1ULL) == 0ULL) {
        ++c;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Cache of results */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;   /* last index for which cache contains a value   */
    static idx_t k = 1;              /* current candidate for the smallest k         */
    static seq_t e = 0;              /* exponent of 2 in k! from current k            */

    /* Allocate or enlarge cache if necessary */
    if (!cache || cached_up_to < n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory, return 0 as an error value */
        cache = tmp;
    }

    /* First call: initialise a(0) */
    if (cached_up_to == 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cached_up_to = 0;
        k = 1;
        e = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t goal = (seq_t)2 * i;            /* target exponent of 2 */
        while (e < goal) {
            ++k;
            e += v2_of(k);
        }
        cache[i] = k;
    }
    cached_up_to = n;

    return cache[n];
}
