/*
 * A204192  Number of n X 3 0..1 arrays with no occurrence of three equal
 *          elements in a row horizontally, vertically or nw-to-se diagonally,
 *          and new values 0..1 introduced in row major order.
 * https://oeis.org/A204192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) -5*a(n-4) -2*a(n-5)
 *            +5*a(n-6) + a(n-7) -2*a(n-8), n>11 Initial values: a(1)=3,
 *            a(2)=18, a(3)=33, a(4)=86, a(5)=231, a(6)=639, a(7)=1724,
 *            a(8)=4697, a(9)=12763, a(10)=34668, a(11)=94209 The function
 *            stores a cache so that calls in any order are O(n) overall
 *            and...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms (index 0 unused) */
    static const seq_t init[12] = {
        0ULL, /* dummy for 0 */
        3ULL, 18ULL, 33ULL, 86ULL, 231ULL,
        639ULL, 1724ULL, 4697ULL, 12763ULL,
        34668ULL, 94209ULL
    };

    /* return directly for the small indices */
    if (n <= 11) {
        return init[n];
    }

    /* Lazy static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size (0 unused) */
    static idx_t  maxn  = 0;          /* largest n computed so far */

    /* First call: allocate cache big enough for all requested terms */
    if (cache == NULL) {
        cap   = (size_t)n + 1;        /* indices 0..n inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – abort */
            /* In the OJ environment, exit 1 is acceptable */
            exit(1);
        }
        /* initialise initial part of the cache */
        for (size_t i = 1; i <= 11 && i < cap; ++i) {
            cache[i] = init[i];
        }
        maxn = 11;
    }

    /* Grow the cache if the new n is larger than current capacity */
    if ((idx_t)n >= (idx_t)cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            exit(1);
        }
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = maxn + 1; k <= n; ++k) {
        /* All arithmetic uses seq_t operations; constants are promoted. */
        cache[k] = 2 * cache[k-1] + 3 * cache[k-2]
                 -       cache[k-3] - 5 * cache[k-4]
                 - 2 * cache[k-5] + 5 * cache[k-6]
                 +       cache[k-7] - 2 * cache[k-8];
    }

    maxn = n;
    return cache[n];
}
