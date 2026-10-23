/*
 * A192112  Monotonic ordering of nonnegative differences 2^i-4^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th distinct non‑negative value of 2^i - 4^j with 0
 *            ≤ i ≤ 40 and 0 ≤ j ≤ ⌊i/2⌋, sorted in increasing order
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq(const void *ap, const void *bp)
{
    const seq_t *a = (const seq_t *)ap;
    const seq_t *b = (const seq_t *)bp;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static seq_t *diffs = NULL;
static size_t diff_cnt = 0;

seq_t a(idx_t n)
{
    /* initialize the table on first call */
    if (diffs == NULL)
    {
        const idx_t max_i = 40;
        size_t total = 0;
        for (idx_t i = 0; i <= max_i; ++i)
            total += (i / 2) + 1;          /* j ranges 0 .. floor(i/2) */

        diffs = (seq_t *)malloc(total * sizeof(seq_t));
        if (!diffs) return 0;              /* out of memory */

        size_t pos = 0;
        for (idx_t i = 0; i <= max_i; ++i)
        {
            seq_t pow_i = (seq_t)(1ULL << i);           /* 2^i */
            for (idx_t j = 0; j <= i / 2; ++j)
            {
                seq_t pow_2j = (seq_t)(1ULL << (2 * j));/* 4^j = 2^(2j) */
                diffs[pos++] = pow_i - pow_2j;
            }
        }

        qsort(diffs, total, sizeof(seq_t), cmp_seq);

        /* remove duplicates in-place */
        size_t uniq = 0;
        for (size_t k = 0; k < total; ++k)
        {
            if (k == 0 || diffs[k] != diffs[uniq - 1])
                diffs[uniq++] = diffs[k];
        }
        diff_cnt = uniq;
    }

    if (n == 0 || n > diff_cnt)
        return 0;   /* out of range; caller should not request such n */

    return diffs[(size_t)(n - 1)];
}
