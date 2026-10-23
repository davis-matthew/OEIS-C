/*
 * A124459  Square array resulting from the bisection of array A124458. (The
 *          other array is A093560.)
 * https://oeis.org/A124459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   Define A[1][1] = 2.
 *   For k ≥ 2:
 *       A[k][1] = 3
 *       A[k][k] = 2
 *       For 1 < i < k:  A[k][i] = A[k-1][i-1] + A[k-1][i]
 *
 *   The sequence a(n) is the elements of the array read row‑by‑row:
 *       a( T(k-1)+i ) = A[k][i]   where  T(k)=k(k+1)/2.
 *
 * This satisfies the OEIS terms a(1)=2, a(2)=3, a(3)=2, a(4)=3, ... .
 */

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> seq_vec;      /* flattened sequence, seq_vec[0] unused */
static std::vector<seq_t> prev_row;     /* last computed row */
static idx_t current_k = 0;             /* number of rows already computed */

#else
#include <stdlib.h>
static seq_t *seq = NULL;          /* flattened sequence, seq[1..] */
static seq_t *prev_row = NULL;     /* last computed row */
static idx_t seq_len = 0;          /* last index stored in seq (T(current_k)) */
static idx_t current_k = 0;        /* number of rows already computed */
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* Ensure seq_vec has a dummy element at index 0 for 1‑based indexing */
    if (seq_vec.empty())
        seq_vec.push_back(0);           /* seq_vec[0] unused */

    /* Return cached value if already computed */
    if (n < seq_vec.size())
        return seq_vec[n];

    /* Generate rows until index n is available */
    while (seq_vec.size() <= n)
    {
        /* New row number */
        idx_t k = current_k + 1;
        size_t klen = (size_t)k;
        std::vector<seq_t> cur_row(klen);

        if (k == 1)            /* first row */
        {
            cur_row[0] = (seq_t)2;
        }
        else
        {
            cur_row[0] = (seq_t)3;              /* first element */
            cur_row[klen - 1] = (seq_t)2;     /* last element */

            /* interior elements: sum of two above */
            for (size_t i = 1; i + 1 < klen; ++i)
                cur_row[i] = prev_row[i - 1] + prev_row[i];
        }

        /* Append new row to the flattened sequence */
        seq_vec.insert(seq_vec.end(), cur_row.begin(), cur_row.end());

        /* Prepare for next iteration */
        prev_row = std::move(cur_row);
        current_k = k;
    }

    return seq_vec[n];

#else   /* C implementation */

    /* Return cached value if already computed */
    if (n <= seq_len)
        return seq[n];

    /* Expand sequence until it contains a(n) */
    while (seq_len < n)
    {
        /* New row number */
        idx_t k = current_k + 1;
        size_t klen = (size_t)k;

        /* Allocate current row */
        seq_t *cur_row = (seq_t *)malloc(klen * sizeof(seq_t));
        if (!cur_row)
            abort();  /* allocation failed – terminate */

        if (k == 1)            /* first row */
        {
            cur_row[0] = (seq_t)2;
        }
        else
        {
            cur_row[0] = (seq_t)3;              /* first element */
            cur_row[klen - 1] = (seq_t)2;     /* last element */

            /* interior elements: sum of two above */
            for (size_t i = 1; i + 1 < klen; ++i)
                cur_row[i] = prev_row[i - 1] + prev_row[i];
        }

        /* Resize sequence array to hold the new entries */
        size_t new_len = (size_t)seq_len + klen;
        seq_t *tmp = (seq_t *)realloc(seq, (new_len + 1) * sizeof(seq_t));
        if (!tmp)
            abort();  /* allocation failed – terminate */
        seq = tmp;

        /* Copy new row into the flattened sequence */
        for (size_t j = 0; j < klen; ++j)
            seq[seq_len + 1 + j] = cur_row[j];

        /* Update bookkeeping */
        seq_len = new_len;
        free(prev_row);
        prev_row = cur_row;
        current_k = k;
    }

    return seq[n];

#endif
}
