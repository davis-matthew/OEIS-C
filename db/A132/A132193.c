/*
 * A132193  Triangle whose n-th row is the list in increasing order of the
 *          integers which are the sum of squares of positive integers with
 *          sum n. The n-th row begins with n and ends with n^2.
 * https://oeis.org/A132193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 410, u16 n <= 9897, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   enumerate_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132193: for each row r (starting at 0) list the distinct
 * sums of squares of all integer partitions of r, sorted increasingly.
 * The flattened sequence is the concatenation of all rows in order.
 * This implementation caches rows as they are computed.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/enumerate_parts.h"

/* Cached row data.  row_arrays[i] points to an array of the distinct sums
 * for row i.  row_counts[i] records the length of that array.  */
static seq_t **row_arrays = NULL;
static size_t *row_counts = NULL;
static size_t cached_rows = 0;

/* Ensure that rows 0 .. upto are computed and cached.  */
static void ensure_rows(idx_t upto)
{
    if (upto < (idx_t)cached_rows)
        return;

    size_t new_size = (size_t)upto + 1;
    row_arrays  = (seq_t**)realloc(row_arrays,  new_size * sizeof(seq_t*));
    row_counts  = (size_t*)realloc(row_counts, new_size * sizeof(size_t));

    for (size_t i = cached_rows; i < new_size; ++i) {
        /* Determine number of distinct sums of squares for row i.  */
        size_t seen_len = i * i + 1;               /* maximum possible sum */
        bool *seen = (bool*)calloc(seen_len, sizeof(bool));
        if (!seen)
            return;                               /* out of memory */

        enumerate_parts((idx_t)i, (idx_t)i, (seq_t)0, seen);

        /* Count the true entries.  */
        size_t cnt = 0;
        for (size_t s = 0; s < seen_len; ++s)
            if (seen[s]) ++cnt;

        /* Allocate the array of sums.  */
        seq_t *arr = (seq_t*)malloc(cnt * sizeof(seq_t));
        if (!arr) {
            free(seen);
            return;                               /* out of memory */
        }
        size_t pos = 0;
        for (size_t s = 0; s < seen_len; ++s)
            if (seen[s]) arr[pos++] = (seq_t)s;

        free(seen);
        row_arrays[i] = arr;
        row_counts[i] = cnt;
    }
    cached_rows = new_size;
}

/* Main function required by the problem statement.  */
seq_t a(idx_t n)
{
    /* Find the row that contains the nth element.  */
    idx_t idx = n;                   /* remaining offset */
    idx_t r = 0;

    while (1) {
        ensure_rows(r);
        size_t cnt = row_counts[r];
        if (idx < (idx_t)cnt) {
            /* The desired element is in this row.  */
            return row_arrays[r][(size_t)idx];
        }
        idx -= (idx_t)cnt;
        ++r;
    }
}
