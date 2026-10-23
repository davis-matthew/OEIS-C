/*
 * A186460  Number of (n+1) X 8 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) for n > 13
 *            Initial values (n = 1 .. 13): 245, 292, 590, 945, 1681, 2377,
 *            4121, 5633, 8893, 11977, 19037, 27133, 43249. The sequence is
 *            computed on demand and memoised
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 40, u64 n <= 94, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    245, 292, 590, 945, 1681, 2377, 4121, 5633, 8893, 11977,
    19037, 27133, 43249
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1, return 0 */
    /* static memoisation storage */
    static idx_t computed = 0;      /* number of values already stored */
    static idx_t cap = 0;           /* allocated capacity */
    static seq_t *cache = NULL;     /* zero based: cache[0] = a(1) */

    /* allocate or grow storage if necessary */
    if (cap < n) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        cap = n;
    }

    /* compute missing values */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 13) {
            val = init_vals[k - 1];
        } else {
            /* use recurrence: a(k) = 2*a(k-1) - 2*a(k-3) + 2*a(k-4) - a(k-5) */
            val = 2 * cache[k - 2];          /* a(k-1) */
            val -= 2 * cache[k - 4];          /* a(k-3) */
            val += 2 * cache[k - 5];          /* a(k-4) */
            val -= cache[k - 6];              /* a(k-5) */
        }
        cache[k - 1] = val;
    }
    computed = n;

    return cache[n - 1];
}
