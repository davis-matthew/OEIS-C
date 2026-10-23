/*
 * A186455  Number of (n+1) X 3 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) for n>8, with
 *            initial values a(1)=21, a(2)=30, a(3)=51, a(4)=88, a(5)=128,
 *            a(6)=197, a(7)=292, a(8)=457
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 46, u64 n <= 100, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but safe to return 0 */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;   /* number of terms computed (1‑based) */

    /* first eight terms */
    const seq_t init[8] = {21, 30, 51, 88, 128, 197, 292, 457};

    /* enlarge cache if necessary */
    if (n > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > computed) {
        for (size_t idx = computed; idx < (size_t)n; ++idx) { /* 0‑based index */
            if (idx < 8) {
                cache[idx] = init[idx];
            } else {
                /* recurrence: a[idx] = 2*a[idx-1] - 2*a[idx-3]
                                 + 2*a[idx-4] - a[idx-5] */
                seq_t term = 2 * cache[idx - 1] + 2 * cache[idx - 4];
                term -= 2 * cache[idx - 3];
                term -= cache[idx - 5];
                cache[idx] = term;
            }
        }
        computed = n;
    }

    return cache[(size_t)n - 1];   /* 1‑based → 0‑based */
}
