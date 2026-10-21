/*
 * A206237  Number of (n+1) X 8 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A206237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 7329*a(n-3) - 7329*a(n-4)   for n > 11
 * Initial values (n = 1 … 11):
 *   a(1) = 294546
 *   a(2) = 943584
 *   a(3) = 3503136
 *   a(4) = 95322432
 *   a(5) = 1122086640
 *   a(6) = 22535636736
 *   a(7) = 649065145152
 *   a(8) = 8026428934128
 *   a(9) = 163621204516992
 *   a(10)= 4735717646406528
 *   a(11)= 58825697658136176
 *
 * The function uses memoisation so that it can handle arbitrary
 * orders of calls and works for all n ≥ 1.  It is O(n) time and
 * O(n) memory.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined; return 0 as a safe default */
        return (seq_t)0;

    /* ---- static cache – memoisation -------------------------------- */
    static seq_t *cache   = NULL;   /* array of a values (1‑based → 0‑based) */
    static idx_t  count   = 0;      /* number of cached terms */

    /* initial values for n = 1 … 11 */
    static const seq_t init[11] = {
        294546ULL,
        943584ULL,
        3503136ULL,
        95322432ULL,
        1122086640ULL,
        22535636736ULL,
        649065145152ULL,
        8026428934128ULL,
        163621204516992ULL,
        4735717646406528ULL,
        58825697658136176ULL
    };

    /* expand the cache if needed */
    if (count < n) {
        size_t new_size = (size_t)n;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        const seq_t coeff = (seq_t)7329;

        /* compute missing terms */
        for (idx_t i = count; i < n; ++i) {
            if (i < 11) {                       /* use the hard‑coded start */
                cache[i] = init[i];
            } else {                             /* recurrence for i >= 11 */
                cache[i] = cache[i-1]
                        + coeff * cache[i-3]
                        - coeff * cache[i-4];
            }
        }
        count = n;
    }

    return cache[n-1];      /* cache[0] holds a(1) */
}
