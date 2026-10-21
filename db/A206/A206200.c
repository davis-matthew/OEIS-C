/*
 * A206200  Number of (n+1) X 3 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 14*a(n-3) - 3*a(n-4) + a(n-5) + a(n-6) +
 *            a(n-7) for n>9. Initial values: a(1)=480, a(2)=1060, a(3)=3008,
 *            a(4)=9464, a(5)=22576, a(6)=67968, a(7)=192808, a(8)=496088,
 *            a(9)=1475768
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 38, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t filled = 0;

    /* initial sequence values */
    const seq_t init[9] =
        {480, 1060, 3008, 9464, 22576, 67968, 192808, 496088, 1475768};

    /* sanity check */
    if (n < 1) return 0;

    /* allocate or grow cache */
    if (cache == NULL) {
        cap = (size_t)n > 10 ? (size_t)n : 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    if (cap < (size_t)n) {
        size_t new_cap = (size_t)n * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute values up to n */
    if (filled < (size_t)n) {
        for (idx_t i = filled + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[(size_t)i - 1] = init[(size_t)i - 1];
            } else {
                /* recurrence:
                   a(n) = 3*a(n-2) + 14*a(n-3) - 3*a(n-4)
                          + a(n-5) + a(n-6) + a(n-7) */
                seq_t val = 3 * cache[(size_t)i - 3];
                val += 14 * cache[(size_t)i - 4];
                val -= 3 * cache[(size_t)i - 5];
                val += cache[(size_t)i - 6];
                val += cache[(size_t)i - 7];
                val += cache[(size_t)i - 8];
                cache[(size_t)i - 1] = val;
            }
        }
        filled = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
