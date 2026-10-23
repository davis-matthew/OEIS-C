/*
 * A192156  Monotonic ordering of nonnegative differences 8^i-3^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth smallest non‑negative difference 8^i - 3^j
 *              for 0 ≤ i ≤ 40, j ≥ 0, with duplicates removed,
 *              offset 1.
 */
#include <stdlib.h>

static int seq_compare(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static cache of the sorted unique differences */
    static seq_t *uniq = NULL;   /* array of unique values */
    static size_t uniq_len = 0;  /* number of unique values */

    if (uniq == NULL)
    {
        /* Pre‑allocate enough space for 0 … 40 */
        const idx_t MAX_I = 40;
        seq_t *pow8  = (seq_t *)malloc((MAX_I + 1) * sizeof(seq_t));
        seq_t *pow3  = (seq_t *)malloc((MAX_I + 1) * sizeof(seq_t));

        /* Compute powers of 8 up to 8^40 or until overflow */
        size_t max_i = 0;
        pow8[0] = (seq_t)1;
        for (size_t i = 1; i <= MAX_I; ++i)
        {
            seq_t t = pow8[i-1] * (seq_t)8;
            if (t < pow8[i-1]) {            /* overflow detected */
                max_i = i-1;
                break;
            }
            pow8[i] = t;
        }
        if (max_i == 0) max_i = MAX_I;      /* no overflow */

        /* Compute powers of 3 up to 3^max_i (should be safe) */
        size_t max_j = 0;
        pow3[0] = (seq_t)1;
        for (size_t j = 1; j <= MAX_I; ++j)
        {
            seq_t t = pow3[j-1] * (seq_t)3;
            if (t < pow3[j-1]) {
                max_j = j-1;
                break;
            }
            pow3[j] = t;
        }
        if (max_j == 0) max_j = MAX_I;      /* no overflow */

        /* Count total number of valid (i,j) pairs */
        size_t total_pairs = 0;
        for (size_t i = 0; i <= max_i; ++i)
        {
            for (size_t j = 0; j <= max_j && pow3[j] <= pow8[i]; ++j)
                ++total_pairs;
        }

        /* Collect all differences */
        seq_t *diffs = (seq_t *)malloc(total_pairs * sizeof(seq_t));
        size_t pos = 0;
        for (size_t i = 0; i <= max_i; ++i)
        {
            for (size_t j = 0; j <= max_j && pow3[j] <= pow8[i]; ++j)
                diffs[pos++] = pow8[i] - pow3[j];
        }

        /* Sort the differences */
        qsort(diffs, total_pairs, sizeof(seq_t), seq_compare);

        /* Count unique values */
        uniq_len = 1;
        for (size_t k = 1; k < total_pairs; ++k)
            if (diffs[k] != diffs[k-1])
                ++uniq_len;

        /* Store unique values in cache */
        uniq = (seq_t *)malloc(uniq_len * sizeof(seq_t));
        uniq[0] = diffs[0];
        size_t out = 1;
        for (size_t k = 1; k < total_pairs; ++k)
            if (diffs[k] != diffs[k-1])
                uniq[out++] = diffs[k];

        /* Free temporary arrays */
        free(diffs);
        free(pow8);
        free(pow3);
    }

    if (n == 0) return (seq_t)0;            /* undefined for offset 0 */
    if ((idx_t)(n-1) < uniq_len)
        return uniq[n-1];
    return (seq_t)0;                        /* n beyond computed range */
}
