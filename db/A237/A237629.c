/*
 * A237629  Number of (n+1) X (n+1) 0..2 arrays with the upper median of every
 *          2 X 2 subblock differing from its horizontal and vertical
 *          neighbors by exactly one.
 * https://oeis.org/A237629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 25*a(n-4) - 100*a(n-6) for n > 12 Initial
 *            values (n = 1 .. 12): 81, 636, 1310, 2012, 4622, 9368, 18344,
 *            44292, 102536, 211268, 402152, 1015572 (a(0) is defined as 0 for
 *            convenience)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* table of the first 12 terms; index 0 is a dummy 0 */
static const seq_t init_vals[13] = {
    0ULL,          /* a(0) dummy */
    81ULL, 636ULL, 1310ULL, 2012ULL, 4622ULL, 9368ULL,
    18344ULL, 44292ULL, 102536ULL, 211268ULL,
    402152ULL, 1015572ULL
};

seq_t a(idx_t n)
{
    if (n == 0) {      /* sequence is defined for n >= 1 only */
        return (seq_t)0;
    }

    /* static cache of computed values */
    static seq_t *cache = NULL;   /* cache[0] is unused */
    static idx_t current = 0;     /* largest n computed so far */
    static size_t capacity = 0;   /* allocated size of cache */

    /* allocate or grow cache if necessary */
    if (capacity <= (size_t)n) {
        size_t needed = (size_t)n + 1;            /* include index 0 */
        seq_t *tmp = NULL;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(needed * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        }
        if (tmp == NULL) {   /* allocation failed – return 0 as safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        capacity = needed;
    }

    /* if the requested value is already computed, return it immediately */
    if (n <= current) {
        return cache[(size_t)n];
    }

    /* compute missing values up to n */
    for (idx_t i = current + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[(size_t)i] = init_vals[(size_t)i];
        } else {
            seq_t t = 4 * cache[(size_t)(i - 2)]
                    + 25 * cache[(size_t)(i - 4)];
            t -= 100 * cache[(size_t)(i - 6)];
            cache[(size_t)i] = t;
        }
    }
    current = n;
    return cache[(size_t)n];
}
