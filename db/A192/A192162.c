/*
 * A192162  Monotonic ordering of nonnegative differences 5^i-4^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Monotonic ordering of nonnegative differences 5^i - 4^j for 0 ≤
 *            i ≤ 40, j ≥ 0 with 5^i ≥ 4^j, sorted ascending
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for the sequence. */
static int initialized = 0;
static seq_t *seq_vals = NULL;
static size_t seq_len  = 0;

/* Comparator used by qsort. */
static int seq_compare (const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Return the n-th element of the sequence (offset 1). */
seq_t a (idx_t n)
{
    if (!initialized)
    {
        /* Generate all valid differences. */
        size_t cap = 2000;
        seq_t *dif = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t cnt  = 0;
        seq_t pow5 = 1;          /* 5^0 */
        for (idx_t i = 0; i <= 40; ++i)
        {
            seq_t pow4 = 1;      /* 4^0 */
            while (pow4 <= pow5)
            {
                if (cnt == cap)
                {
                    cap <<= 1;
                    dif = (seq_t *)realloc(dif, cap * sizeof(seq_t));
                }
                dif[cnt++] = pow5 - pow4;

                seq_t next_pow4 = pow4 * 4;
                if (next_pow4 < pow4)           /* overflow of unsigned */
                    break;
                if (next_pow4 > pow5)           /* would exceed 5^i */
                    break;
                pow4 = next_pow4;
            }

            if (i == 40) break;
            seq_t next_pow5 = pow5 * 5;
            if (next_pow5 < pow5)                 /* overflow */
                break;
            pow5 = next_pow5;
        }

        /* Sort the differences. */
        qsort(dif, cnt, sizeof(seq_t), seq_compare);

        /* Remove duplicates. */
        size_t uniq = 0;
        for (size_t i = 0; i < cnt; ++i)
        {
            if (uniq == 0 || dif[i] != dif[uniq - 1])
                dif[uniq++] = dif[i];
        }

        /* Store the final sequence. */
        seq_vals = (seq_t *)malloc(uniq * sizeof(seq_t));
        for (size_t i = 0; i < uniq; ++i)
            seq_vals[i] = dif[i];
        seq_len = uniq;

        /* Free temporary buffer. */
        free(dif);
        initialized = 1;
    }

    if (n == 0)
        return 0;                 /* offset 1; a(0) is undefined */
    if ((idx_t)(n - 1) < seq_len)
        return seq_vals[n - 1];
    return 0;                     /* outside of precomputed range */
}
