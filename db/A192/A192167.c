/*
 * A192167  Monotonic ordering of nonnegative differences 4^i-8^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest non‑negative difference 4^i – 8^j,
 *            where 0 ≤ i ≤ 40, 0 ≤ j ≤ ⌊2i/3⌋. The values are sorted
 *            ascending and duplicates removed. OEIS offset is 1
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   compare_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/compare_seq.h"

seq_t a(idx_t n)
{
    /* Lazy static initialization of all terms up to the maximum 4^i–8^j
       that fits into seq_t.  The number of terms is very small
       (≈600), so this is inexpensive and safe for multiple calls. */
    static bool initialized = false;
    static seq_t *values      = NULL;
    static size_t values_cnt  = 0;
    static size_t values_cap  = 0;

    if (!initialized) {
        /* Pre‑compute powers of 4 and 8 up to exponent 40.
           Use unsigned __int128 for the intermediate values to avoid
           overflow on 64‑bit seq_t.  */
        unsigned __int128 pow4[41];
        unsigned __int128 pow8[41];

        pow4[0] = 1;
        for (idx_t i = 1; i <= 40; ++i)
            pow4[i] = pow4[i - 1] * (unsigned __int128)4;

        pow8[0] = 1;
        for (idx_t j = 1; j <= 40; ++j)
            pow8[j] = pow8[j - 1] * (unsigned __int128)8;

        /* Estimate a generous capacity; the actual number of terms is
           far smaller (~600), but this keeps reallocations unlikely. */
        values_cap = 2048;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        values_cnt = 0;

        /* Determine the numeric limit that can be represented in seq_t
           if seq_t is 8 bytes or smaller.  For larger seq_t (e.g. 128‑bit
           or arbitrary‑precision) we accept all values. */
        unsigned __int128 limit = 0;
        if (sizeof(seq_t) == 1)
            limit = UINT8_MAX;
        else if (sizeof(seq_t) == 2)
            limit = UINT16_MAX;
        else if (sizeof(seq_t) == 4)
            limit = UINT32_MAX;
        else if (sizeof(seq_t) == 8)
            limit = (unsigned __int128)UINT64_MAX;

        /* Generate all non‑negative differences and store those that
           fit into seq_t. */
        for (idx_t i = 0; i <= 40; ++i) {
            unsigned __int128 pi = pow4[i];
            idx_t max_j = (2 * i) / 3;          /* 4^i ≥ 8^j */
            for (idx_t j = 0; j <= max_j; ++j) {
                unsigned __int128 pj = pow8[j];
                unsigned __int128 diff = pi - pj;   /* non‑negative by construction */

                if (sizeof(seq_t) <= 8 && diff > limit)
                    continue;                     /* too large for 64‑bit seq_t */

                if (values_cnt >= values_cap) {
                    values_cap *= 2;
                    values = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
                }
                values[values_cnt++] = (seq_t)diff;
            }
        }

        /* Sort and deduplicate. */
        qsort(values, values_cnt, sizeof(seq_t), compare_seq);
        size_t uniq = 0;
        for (size_t i = 0; i < values_cnt; ++i) {
            if (i == 0 || values[i] != values[i - 1])
                values[uniq++] = values[i];
        }
        values_cnt = uniq;

        initialized = true;
    }

    /* Return the requested term or 0 if out of bounds (n ≤ 0 or beyond
       the pre‑computed range). */
    if (n == 0)
        return 0;
    if ((idx_t)(values_cnt) < (n - 1))
        return 0;
    return values[n - 1];
}
