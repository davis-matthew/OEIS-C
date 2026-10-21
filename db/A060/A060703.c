/*
 * A060703  Negative values of 2*x*y^4 + x^2*y^3 - 2*x^3*y*2 - x^4*y - y^5 +
 *          2*y for x,y >= 0.
 * https://oeis.org/A060703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest positive value |2*x*y^4 + x^2*y^3 -
 *            2*x^3*y^2 - x^4*y - y^5 + 2*y| for all non‑negative integer
 *            pairs (x, y); the values are sorted in increasing order and
 *            duplicates are removed. a(0) is the first term listed in the
 *            OEIS entry
 * Range:     u8 none, u16 none, u32 n <= 36, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_cap_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_cap_seq_t.h"

/* Comparison function for qsort, compatible with both integer
   and arbitrary‑precision seq_t types. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Cached sequence values */
static seq_t *cached_vals = NULL;
static size_t cached_cap = 0;
static size_t cached_len = 0;

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (cached_len == 0) {
        /* Bounds for x and y that guarantee >10 000 terms.
           Adjust if a larger range is required for higher n. */
        const idx_t LIMIT = 2500;
        const size_t EST_TOTAL = (size_t)LIMIT * (LIMIT + 1);

        ensure_cap_seq_t((void **)&cached_vals, &cached_cap, EST_TOTAL);
        cached_len = 0;

        for (idx_t y = 0; y <= LIMIT; ++y) {
            int64_t yy = (int64_t)y;
            int64_t y2 = yy * yy;
            int64_t y3 = y2 * yy;
            int64_t y4 = y3 * yy;
            int64_t y5 = y4 * yy;

            for (idx_t x = 0; x <= LIMIT; ++x) {
                int64_t xx = (int64_t)x;
                int64_t x2 = xx * xx;
                int64_t x3 = x2 * xx;
                int64_t x4 = x3 * xx;

                /* Evaluate the polynomial. */
                int64_t poly = 2 * xx * y4
                           + x2 * y3
                           - 2 * x3 * y2
                           - x4 * yy
                           - y5
                           + 2 * yy;

                if (poly < 0) {
                    cached_vals[cached_len++] = (seq_t)(-poly);
                }
            }
        }

        /* Sort the list. */
        qsort(cached_vals, cached_len, sizeof(seq_t), seq_cmp);

        /* Remove duplicates in place. */
        size_t uniq = 0;
        for (size_t i = 0; i < cached_len; ++i) {
            if (i == 0 || cached_vals[i] != cached_vals[i - 1]) {
                if (uniq < i) cached_vals[uniq] = cached_vals[i];
                ++uniq;
            }
        }
        cached_len = uniq;
    }

    if ((idx_t)n >= cached_len) return 0;
    return cached_vals[(size_t)n];
}
