/*
 * A192200  Monotonic ordering of nonnegative differences 9^i-5^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 34, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Monotonic ordering of nonnegative differences 9^i-5^j for 0<=i<=40, j>=0 */
#include <stdlib.h>
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* static buffers for one‑time precomputation */
    static seq_t uniq[6000];
    static size_t uniq_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* temporary buffer for all differences */
        static seq_t diffs[6000];
        size_t difflen = 0;

        seq_t pow9 = 1;                    /* 9^i */
        for (idx_t i = 0; i <= 40; ++i) {
            seq_t pow5 = 1;                /* 5^j */

            /* collect all nonnegative differences for this i */
            while (pow5 <= pow9) {
                if (difflen < (size_t)sizeof(diffs)/sizeof(diffs[0]))
                    diffs[difflen++] = pow9 - pow5;
                pow5 *= 5;
            }

            pow9 *= 9;
        }

        /* sort the differences */
        qsort(diffs, difflen, sizeof(seq_t), seq_cmp);

        /* deduplicate while preserving sorted order */
        for (size_t k = 0; k < difflen; ++k) {
            if (k == 0 || diffs[k] != diffs[k - 1])
                uniq[uniq_len++] = diffs[k];
        }

        initialized = 1;
    }

    if (n == 0 || n > uniq_len)
        return 0;
    return uniq[n - 1];
}
