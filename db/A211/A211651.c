/*
 * A211651  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^3 >= x^3 + y^3.
 * https://oeis.org/A211651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with * w,x,y ∈ {1,…,n}
 *            and w³ ≥ x³ + y³. * Computed by iterating over w and for each w
 *            counting * (x,y) pairs using a two‑pointer technique.
 *            Complexity: O(n²)
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t cubes_cap = 0;
static uint64_t *cubes_buf = NULL;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Ensure cube array large enough */
    if (cubes_cap < n) {
        uint64_t *new_buf = (uint64_t *)realloc(cubes_buf, (n + 1) * sizeof(uint64_t));
        if (!new_buf) return 0;          /* allocation failure – unlikely */
        cubes_buf = new_buf;
        cubes_cap = n;
        cubes_buf[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
            cubes_buf[i] = (uint64_t)i * i * i;
    }

    seq_t total = 0;

    for (idx_t w = 1; w <= n; ++w) {
        uint64_t limit = cubes_buf[w];
        idx_t y = n;                      /* largest possible y for this w */

        for (idx_t x = 1; x <= n && cubes_buf[x] <= limit; ++x) {
            /* move y downward while the condition is violated */
            while (y > 0 && cubes_buf[x] + cubes_buf[y] > limit)
                --y;
            total += (seq_t)y;             /* add number of admissible y's */
        }
    }

    return total;
}
