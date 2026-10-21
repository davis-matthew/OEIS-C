/*
 * A211653  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^4>x^4+y^4.
 * https://oeis.org/A211653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with * w,x,y ∈ {1,…,n}
 *            and w^4 > x^4 + y^4. * The implementation uses a two‑pointer
 *            counting algorithm for each w, * giving O(n²) time with small
 *            constants
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Dynamically extend a static array of fourth powers up to max n seen. */
    static uint64_t *pow4 = NULL;
    static idx_t pow4_size = 0;

    if ((idx_t)(n + 1) > pow4_size) {
        size_t new_size = (size_t)(n + 1);
        uint64_t *tmp = (uint64_t *)realloc(pow4, new_size * sizeof(uint64_t));
        if (!tmp) return 0;           /* allocation failure – return 0 */
        pow4 = tmp;
        /* compute fourth powers from previous size up to n */
        for (idx_t i = pow4_size; i <= n; ++i) {
            uint64_t v = (uint64_t)i;
            pow4[i] = v * v * v * v;  /* i^4 fits in 64 bits for i ≤ 10000 */
        }
        pow4_size = new_size;
    }

    uint64_t N = (uint64_t)n;
    uint64_t total = 0;

    for (uint64_t w = 1; w <= N; ++w) {
        uint64_t limit = pow4[w];    /* w^4 */
        uint64_t y = N;              /* maximal y candidate */

        for (uint64_t x = 1; x <= N; ++x) {
            /* Decrease y until pow4[x] + pow4[y] < limit */
            while (y > 0 && pow4[x] + pow4[y] >= limit) {
                --y;
            }
            total += y;              /* y valid values: 1..y */
        }
    }

    /* The result fits in seq_t whenever it does not overflow its type. */
    return (seq_t)total;
}
