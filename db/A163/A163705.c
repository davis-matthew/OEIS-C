/*
 * A163705  Number of n X 3 binary arrays with all 1s connected, a path of 1s
 *          from left column to lower right corner, and no 1 having more than
 *          two 1s adjacent.
 * https://oeis.org/A163705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 11*a(n-3) + a(n-4) * - 9*a(n-5) +
 *            7*a(n-6) - a(n-7) - 2*a(n-8) + a(n-9) for n>=12 * Initial
 *            values: a(1)=1, a(2)=7, a(3)=28, a(4)=80, a(5)=200, * a(6)=469,
 *            a(7)=1064, a(8)=2359, a(9)=5144, * a(10)=11091, a(11)=23739
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 56, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* the sequence is defined for n≥1 only */

    /* static cache of already computed values (index 0 unused) */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of elements stored (indices 0..size-1) */

    /* initialize cache with first 11 terms */
    if (!cache) {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure – return 0 as fallback */

        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 7;
        cache[3] = 28;
        cache[4] = 80;
        cache[5] = 200;
        cache[6] = 469;
        cache[7] = 1064;
        cache[8] = 2359;
        cache[9] = 5144;
        cache[10] = 11091;
        cache[11] = 23739;

        size = 12;               /* indices 0..11 are initialized */
    } else if ((idx_t)size <= n) { /* need more terms? */
        /* grow allocation to at least n+1 elements */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure – return 0 as fallback */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (size_t i = size; i <= (size_t)n; ++i) {
            if (i <= 11) continue;  /* initial terms already present */

            seq_t pos = 6 * cache[i - 1] +
                         11 * cache[i - 3] +
                         cache[i - 4] +
                         7 * cache[i - 6] +
                         cache[i - 9];
            seq_t neg = 13 * cache[i - 2] +
                         9 * cache[i - 5] +
                         2 * cache[i - 8] +
                         cache[i - 7];
            cache[i] = pos - neg;     /* guaranteed positive for n≥12 */
        }
        size = n + 1;
    }

    return cache[n];
}
