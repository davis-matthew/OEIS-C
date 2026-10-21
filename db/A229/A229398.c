/*
 * A229398  Number of n X 4 0..2 arrays with top left element 0, horizontal
 *          differences mod 3 never 1, vertical differences mod 3 never -1,
 *          and antidiagonal differences never 0.
 * https://oeis.org/A229398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 47*a(n-3) - 55*a(n-4) + 47*a(n-5)
 *            - 28*a(n-6) + 9*a(n-7) - a(n-8), with a(1)=8, a(2)=34, a(3)=153,
 *            a(4)=711, a(5)=3357, a(6)=15952, a(7)=75965, a(8)=362012
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t base[8] = {
    (seq_t)8, (seq_t)34, (seq_t)153, (seq_t)711,
    (seq_t)3357, (seq_t)15952, (seq_t)75965, (seq_t)362012
};

seq_t a(idx_t n)
{
    /* n is 1‑based; return 0 for n < 1 */
    if (n == 0) return (seq_t)0;

    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t  computed = 0;   /* number of terms stored */

    /* ensure capacity up to n */
    if (n > cap) {
        size_t newcap = n;
        if (newcap < 64) newcap = 64;          /* small initial size */
        if (newcap < cap * 2) newcap = cap * 2;/* double when needed */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i - 1] = base[i - 1];
        } else {
            /* compute a(i) using the 8‑term recurrence */
            seq_t s1 = 9 * cache[i - 2] +       /* a(i-1) */
                       47 * cache[i - 4] +      /* a(i-3) */
                       47 * cache[i - 6] +      /* a(i-5) */
                       9 * cache[i - 8];       /* a(i-7) */

            seq_t s2 = 28 * cache[i - 3] +      /* a(i-2) */
                       55 * cache[i - 5] +      /* a(i-4) */
                       28 * cache[i - 7] +      /* a(i-6) */
                       cache[i - 9];           /* a(i-8) */

            cache[i - 1] = s1 - s2;
        }
    }
    computed = n;
    return cache[n - 1];
}
