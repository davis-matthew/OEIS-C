/*
 * A228082  Numbers that are of the form k + sum of binary digits of k for
 *          some nonnegative integer k.
 * https://oeis.org/A228082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 189, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence A228082: numbers that can be written as k + popcount(k) for some nonnegative integer k */

/* Helper comparison for qsort – works for unsigned integers and most big‑int types */
static int compare_seq(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

/* Static storage for the generated sequence */
static seq_t *seq = NULL;    /* sorted unique values */
static size_t seq_len = 0;   /* number of values stored */

/* Current largest k processed (exclusive) */
static idx_t k_last = 0;
/* Current batch size to generate */
static idx_t batch = 4096;

/* Extend the stored sequence until it contains at least n entries */
static void extend_to_n(idx_t n)
{
    while (seq_len < n)
    {
        idx_t k_start = k_last;
        idx_t k_end   = k_start + batch;

        size_t buf_size = (size_t)(k_end - k_start);
        seq_t *buf = (seq_t *)malloc(buf_size * sizeof(seq_t));
        if (!buf) exit(1);

        size_t buf_len = 0;
        for (idx_t k = k_start; k < k_end; ++k)
        {
            uint64_t temp = (uint64_t)k + (uint64_t)__builtin_popcountll(k);

            /* Skip values that do not fit into seq_t when seq_t is an unsigned integer type */
            if (sizeof(seq_t) <= sizeof(uint64_t))
            {
                seq_t max_val = (seq_t)-1;
                if (temp > (uint64_t)max_val)
                    continue;
            }

            buf[buf_len++] = (seq_t)temp;
        }

        /* Sort the buffer */
        qsort(buf, buf_len, sizeof(seq_t), compare_seq);

        /* Remove duplicates inside the buffer */
        size_t uniq_len = 0;
        seq_t *uniq_buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        if (!uniq_buf) exit(1);

        for (size_t i = 0; i < buf_len; ++i)
        {
            if (i == 0 || buf[i] != buf[i - 1])
                uniq_buf[uniq_len++] = buf[i];
        }

        /* Merge the existing sequence with the new unique values */
        size_t merged_len = seq_len + uniq_len;
        seq_t *merged = (seq_t *)malloc(merged_len * sizeof(seq_t));
        if (!merged) exit(1);

        size_t i = 0, j = 0, k = 0;
        while (i < seq_len && j < uniq_len)
        {
            if (seq[i] < uniq_buf[j])
                merged[k++] = seq[i++];
            else if (seq[i] > uniq_buf[j])
                merged[k++] = uniq_buf[j++];
            else {            /* equal – keep one copy */
                merged[k++] = seq[i++];
                ++j;
            }
        }
        while (i < seq_len) merged[k++] = seq[i++];
        while (j < uniq_len) merged[k++] = uniq_buf[j++];

        /* Resize to the actual number of unique elements */
        seq_len = k;
        seq = (seq_t *)realloc(merged, seq_len * sizeof(seq_t));
        if (!seq) exit(1);

        /* Clean up */
        free(buf);
        free(uniq_buf);

        /* Prepare for next batch */
        k_last = k_end;
        if (batch < 1024 * 1024) batch *= 2;
    }
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but return 0 */
    extend_to_n(n);
    return seq[n - 1];
}
