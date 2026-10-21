/*
 * A056538  Irregular triangle read by rows: row n lists the divisors of n in
 *          decreasing order.
 * https://oeis.org/A056538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1457, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   count_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A056538.
   The sequence is obtained by reading a triangle whose
   row *r* lists the divisors of *r* in decreasing order.
   The n‑th term is the (n‑th smallest) divisor of the row
   that contains the n‑th entry.  The code keeps a running
   cumulative array of divisor counts to find the correct
   row and position in O(√r) for each query. */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/count_divisors.h"

/* Forward declaration of helper functions */
static idx_t count_divisors(idx_t n);
static int cmp_idx(const void *a, const void *b);

/* Cumulative sum of divisor counts.
   cum[0] = 0                                            */
static idx_t *cum = NULL;
static size_t cum_len = 0;   /* number of entries stored   */
static size_t cum_cap = 0;   /* allocated capacity        */

/* Ensure cumulative array contains rows up to a cumulative
   sum ≥ target.  This function grows the array as needed. */
static void ensure_cumulative(idx_t target)
{
    /* Initialise on first use */
    if (cum == NULL) {
        cum_cap = 4;
        cum = (idx_t*)malloc(cum_cap * sizeof(idx_t));
        assert(cum != NULL);
        cum[0] = 0;
        cum_len = 1;
    }

    /* Extend until we have enough rows */
    while (cum_len == 0 || cum[cum_len - 1] < target) {
        idx_t r = (idx_t)(cum_len);          /* the next row number   */
        idx_t dcnt = count_divisors(r);

        if (cum_len >= cum_cap) {            /* grow capacity          */
            size_t new_cap = cum_cap * 2;
            idx_t *tmp = (idx_t*)realloc(cum, new_cap * sizeof(idx_t));
            assert(tmp != NULL);
            cum = tmp;
            cum_cap = new_cap;
        }

        cum[cum_len] = cum[cum_len - 1] + dcnt;
        cum_len++;
    }
}

/* Comparator for qsort on idx_t. */
static int cmp_idx(const void *a, const void *b)
{
    idx_t ia = *(const idx_t*)a;
    idx_t ib = *(const idx_t*)b;
    if (ia < ib) return -1;
    if (ia > ib) return  1;
    return 0;
}

/* Return the n-th term of sequence A056538. */
seq_t a(idx_t n)
{
    /* Find the row that contains the n-th entry. */
    ensure_cumulative(n);
    idx_t r = (idx_t)(cum_len - 1);          /* row number      */
    idx_t prev_sum = cum[r - 1];             /* sum up to previous row */
    idx_t k = n - prev_sum;                  /* position in this row (1‑based) */

    /* Count divisors of the row when building cumulative array.  */
    idx_t dcnt = count_divisors(r);

    /* Generate all divisors of r in ascending order. */
    idx_t *divs = (idx_t*)malloc(dcnt * sizeof(idx_t));
    assert(divs != NULL);
    idx_t pos = 0;
    for (idx_t i = 1; (uint64_t)i * (uint64_t)i <= r; ++i) {
        if (r % i == 0) {
            divs[pos++] = i;
            if (i != r / i) {
                divs[pos++] = r / i;
            }
        }
    }
    assert(pos == dcnt);
    qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

    /* kth smallest divisor (1‑based). */
    idx_t small_div = divs[k - 1];

    free(divs);

    /* kth largest divisor = r / small_div. */
    return (seq_t)(r / small_div);
}
