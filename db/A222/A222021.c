/*
 * A222021  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..2 array extended with zeros and
 *          convolved with 1,3,3,1.
 * https://oeis.org/A222021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-7)+a(n-9) for n>=10; a(1)=2, a(2)=3,
 *            a(3)=4, a(4)=6, a(5)=10, a(6)=15, a(7)=23, a(8)=36, a(9)=55
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 51, u64 n <= 103,
 *            u128 n <= 206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_known = 0;

    /* base values for 1..9 (index 0 unused) */
    static const seq_t base[10] = {
        0,  /* unused */
        2,  /* a(1) */
        3,  /* a(2) */
        4,  /* a(3) */
        6,  /* a(4) */
        10, /* a(5) */
        15, /* a(6) */
        23, /* a(7) */
        36, /* a(8) */
        55  /* a(9) */
    };

    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* grow cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – behaviour undefined here */
            return 0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_known + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[(size_t)i] = base[(size_t)i];
        } else {
            cache[(size_t)i] = cache[(size_t)(i-1)] + cache[(size_t)(i-3)]
                            + cache[(size_t)(i-7)] + cache[(size_t)(i-9)];
        }
    }
    max_known = n;
    return cache[(size_t)n];
}
