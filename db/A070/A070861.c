/*
 * A070861  Triangle of all possible distinct numbers obtained as a product of
 *          distinct numbers from 1..n.
 * https://oeis.org/A070861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the (n‑th) term of the flattened triangle where row r
 *            consists of all distinct products of subsets of {1,2,…,r},
 *            sorted in ascending order
 * Range:     u8 n <= 51, u16 n <= 330, u32 n <= 3005, u64 n <= 9984,
 *            u128 n <= 9984, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds each row from the previous one and
   searches forward until the cumulative number of terms exceeds
   n.  Uses different allocation strategies for C and C++ builds
   so that seq_t may be a primitive integer type or an
   arbitrary‑precision integer class. */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#else
#include <stdlib.h>
#endif

/* Comparator for seq_t values (required only in the C build). */
#ifndef __cplusplus
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb) ? 1 : 0;
}
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector */
    std::vector<seq_t> row;
    row.reserve(1);
    row.push_back(seq_t(1));            /* row 0: {1} */

    idx_t cumulative = 0;                /* number of terms emitted so far */
    idx_t r = 0;                          /* current row index (0‑based) */

    while (true)
    {
        /* If the desired index lies in this row, return it. */
        if (cumulative + static_cast<idx_t>(row.size()) > n)
        {
            return row[static_cast<size_t>(n - cumulative)];
        }

        cumulative += static_cast<idx_t>(row.size());
        ++r;                              /* prepare next row with factor r+1 */

        seq_t factor = static_cast<seq_t>(r);  /* factor = r (since r starts at 1) */
        std::vector<seq_t> next;
        next.reserve(row.size() * 2);

        for (const seq_t &v : row)
        {
            next.push_back(v);           /* subset without new factor */
            next.push_back(v * factor);  /* subset including new factor */
        }

        std::sort(next.begin(), next.end());
        auto it = std::unique(next.begin(), next.end());
        next.erase(it, next.end());

        row.swap(next);                  /* replace current row with the new one */
    }
#else
    /* C implementation using malloc / qsort */
    seq_t *row = (seq_t *)malloc(sizeof(seq_t));
    if (!row) return (seq_t)0;
    row[0] = (seq_t)1;                   /* row 0: {1} */

    size_t row_sz = 1;
    idx_t cumulative = 0;
    idx_t r = 0;

    while (1)
    {
        /* If the desired index lies in this row, return it. */
        if (cumulative + (idx_t)row_sz > n)
        {
            seq_t ans = row[(size_t)(n - cumulative)];
            free(row);
            return ans;
        }

        cumulative += (idx_t)row_sz;
        ++r;                              /* prepare next row with factor r+1 */
        seq_t factor = (seq_t)r;

        size_t next_cap = row_sz * 2;
        seq_t *next = (seq_t *)malloc(next_cap * sizeof(seq_t));
        if (!next) { free(row); return (seq_t)0; }

        size_t idx = 0;
        for (size_t i = 0; i < row_sz; ++i)
        {
            next[idx++] = row[i];          /* old subset */
            next[idx++] = row[i] * factor; /* new subset including factor */
        }

        qsort(next, next_cap, sizeof(seq_t), cmp_seq_t);

        size_t unique = 0;
        for (size_t i = 0; i < next_cap; ++i)
        {
            if (i == 0 || cmp_seq_t(&next[i], &next[i - 1]) != 0)
                next[unique++] = next[i];
        }

        free(row);
        row = (seq_t *)realloc(next, unique * sizeof(seq_t));
        if (!row) row = next;   /* realloc failed, keep the old pointer */
        row_sz = unique;
    }
#endif
}
