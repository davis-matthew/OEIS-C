/*
 * A043597  Numbers whose base-4 representation has exactly 6 runs.
 * https://oeis.org/A043597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_seq, ncr5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation has exactly six runs. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/ncr5.h"

static seq_t *seq_array = NULL;
static size_t seq_count = 0;
static int seq_precomputed = 0;

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x > *y) - (*x < *y);
}

/* precompute all numbers up to a modest length with 6 runs */
static void precompute_seq(void)
{
    const unsigned L_min = 6;
    const unsigned L_max = 8;      /* enough for n up to ~10,000 */
    size_t capacity = 0;
    for (unsigned L = L_min; L <= L_max; ++L)
        capacity += ncr5(L-1) * 729;   /* first digit 3 choices, each of the 5 others 3 choices */

    seq_array = alloc_seq(capacity);
    seq_count = 0;

    for (unsigned L = L_min; L <= L_max; ++L) {
        /* iterate all compositions of L into 6 positive parts */
        for (unsigned i1 = 1; i1 <= L-5; ++i1)
        for (unsigned i2 = i1+1; i2 <= L-4; ++i2)
        for (unsigned i3 = i2+1; i3 <= L-3; ++i3)
        for (unsigned i4 = i3+1; i4 <= L-2; ++i4)
        for (unsigned i5 = i4+1; i5 <= L-1; ++i5)
        {
            unsigned len[6];
            len[0] = i1;
            len[1] = i2-i1;
            len[2] = i3-i2;
            len[3] = i4-i3;
            len[4] = i5-i4;
            len[5] = L-i5;

            /* iterate all 4·3⁵ digit patterns, ensuring the first digit != 0 */
            for (int d1 = 1; d1 < 4; ++d1)
            for (int d2 = 0; d2 < 4; ++d2) if (d2 != d1)
            for (int d3 = 0; d3 < 4; ++d3) if (d3 != d2)
            for (int d4 = 0; d4 < 4; ++d4) if (d4 != d3)
            for (int d5 = 0; d5 < 4; ++d5) if (d5 != d4)
            for (int d6 = 0; d6 < 4; ++d6) if (d6 != d5)
            {
                seq_t val = 0;
                int digits[6] = { d1, d2, d3, d4, d5, d6 };
                for (int r = 0; r < 6; ++r) {
                    unsigned l = len[r];
                    for (unsigned k = 0; k < l; ++k)
                        val = val * 4 + digits[r];
                }
                seq_array[seq_count++] = val;
            }
        }
    }

    /* sort the list */
    qsort(seq_array, seq_count, sizeof(seq_t), cmp_seq);
    seq_precomputed = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; below 1 is undefined */
    if (!seq_precomputed) precompute_seq();
    if ((idx_t)seq_count < n) return 0;
    return seq_array[n - 1];
}
