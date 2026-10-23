/*
 * A196931  Triangle read by rows in which row n lists in nondecreasing order
 *          the smallest part of every partition of n.
 * https://oeis.org/A196931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2713, u16 n <= 2713, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   count_partitions_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A196931: a(0)=0; thereafter, flatten the rows
   in which row n lists in non‑decreasing order the smallest part
   of every partition of n. This routine does not hard‑code values
   but builds the table on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_partitions_min.h"

/* ------------------------------------------------------------ */
/*  Partition number: p(n) using the pentagonal recurrence.    */
/* ------------------------------------------------------------ */
static uint64_t partition_count(idx_t n)
{
    if (n == 0) return 1;
    uint64_t *p = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!p) return 0;
    p[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        __int128 sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            sum += ((k & 1) ? 1 : -1) * (int64_t)p[i - pent1];
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            sum += ((k & 1) ? 1 : -1) * (int64_t)p[i - pent2];
        }
        p[i] = (uint64_t)sum;
    }
    uint64_t res = p[n];
    free(p);
    return res;
}

/* ------------------------------------------------------------ */
/*  Global state: arrays of p(n) and cumulative sums.          */
/* ------------------------------------------------------------ */
static idx_t *partitions   = NULL; /* p(i) in idx_t */
static idx_t *cumulative   = NULL; /* Σ_{k=0..i} p(k) */
static size_t precomp_len  = 0;     /* number of rows stored (==size of arrays) */

/* Ensure data for rows 0..max_row (inclusive) are available. */
static void extend_precomputation(idx_t max_row)
{
    if (max_row + 1 <= precomp_len) return;

    size_t new_len = (size_t)(max_row + 1);
    idx_t *new_parts = (idx_t *)realloc(partitions, new_len * sizeof(idx_t));
    idx_t *new_cum   = (idx_t *)realloc(cumulative, new_len * sizeof(idx_t));
    if (!new_parts || !new_cum) {
        /* Allocation failed: keep old data (which are still valid). */
        if (new_parts) free(new_parts);
        if (new_cum)   free(new_cum);
        return;
    }
    partitions = new_parts;
    cumulative = new_cum;

    for (size_t i = precomp_len; i <= max_row; ++i) {
        uint64_t p_val = partition_count((idx_t)i);
        partitions[i] = (idx_t)p_val;
        if (i == 0)
            cumulative[0] = (idx_t)p_val;
        else
            cumulative[i] = cumulative[i - 1] + partitions[i];
    }
    precomp_len = new_len;
}

/* ------------------------------------------------------------ */
/*  Count partitions of "rem" with all parts >= "min_part".    */

/* ------------------------------------------------------------ */
/*  Main sequence function.                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure we have at least row 0 precomputed. */
    if (precomp_len == 0)
        extend_precomputation(0);

    /* Grow precomputation until the cumulative sum passes n. */
    while (cumulative[precomp_len - 1] <= n)
        extend_precomputation(precomp_len + 10);

    /* Find the row r such that cumulative[r-1] <= n < cumulative[r]. */
    idx_t r = 0;
    while (cumulative[r] <= n) r++;

    idx_t prev_cum = (r == 0) ? 0 : cumulative[r - 1];
    idx_t pos_in_row = n - prev_cum; /* 0‑based position in row r */

    /* Generate the list of smallest parts for row r in sorted order. */
    uint64_t answer = 0;
    uint64_t acc_cnt = 0;
    for (uint64_t s = 1; s <= r; ++s) {
        uint64_t cnt = count_partitions_min(r - s, s);
        acc_cnt += cnt;
        if (pos_in_row < acc_cnt) {
            answer = s;
            break;
        }
    }
    return (seq_t)answer;
}
