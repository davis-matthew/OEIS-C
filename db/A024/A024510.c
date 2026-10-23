/*
 * A024510  Positions of even numbers in A004431 (sums of 2 distinct nonzero
 *          squares).
 * https://oeis.org/A024510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024510: positions of even numbers in A004431 (sums of two distinct non‑zero squares).
 *
 * The sequence is defined as follows:
 *   - Compute all distinct sums i² + j² with 1 ≤ i < j.
 *   - Sort the sums, remove duplicates.
 *   - The resulting list is indexed (1‑based); a(n) is the index of the
 *     n‑th even term in that list.
 * This implementation builds the list on demand, extending it in blocks
 * until the requested index is available.  The method is O(limit² log limit)
 * for the current block size and is fast enough for n ≲ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort on uint64_t values. */
static int cmp_u64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Rebuild the position table up to the given limit on i and j (1 ≤ i<j≤limit).
 * The array *pos_out will contain the 1‑based indices of the even sums, *pos_len_out
 * its current length, and *pos_cap_out its allocated capacity.  The new limit
 * is stored in *cur_limit_out.  Any previously allocated array is freed.
 */
static void recompute(idx_t limit,
                      seq_t **pos_out,
                      size_t *pos_len_out,
                      size_t *pos_cap_out,
                      idx_t *cur_limit_out)
{
    /* free old array if present */
    if (*pos_out) {
        free(*pos_out);
        *pos_out = NULL;
    }

    /* number of all unordered pairs (i,j), i<j */
    size_t sum_cap = (size_t)limit * (limit - 1) / 2;
    uint64_t *sums = (uint64_t *)malloc(sum_cap * sizeof(uint64_t));
    if (!sums) exit(EXIT_FAILURE);

    /* collect all sums */
    size_t idx = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        uint64_t ii = (uint64_t)i * (uint64_t)i;
        for (idx_t j = i + 1; j <= limit; ++j) {
            sums[idx++] = ii + (uint64_t)j * (uint64_t)j;
        }
    }

    /* sort all sums */
    qsort(sums, sum_cap, sizeof(uint64_t), cmp_u64);

    /* count unique sums and even ones */
    size_t uniq = 0;
    size_t even_cnt = 0;
    for (size_t k = 0; k < sum_cap; ++k) {
        if (k == 0 || sums[k] != sums[k - 1]) {
            if ((sums[k] & 1ULL) == 0) even_cnt++;
            uniq++;
        }
    }

    /* allocate array for positions of even sums */
    seq_t *newpos = (seq_t *)malloc(even_cnt * sizeof(seq_t));
    if (!newpos) exit(EXIT_FAILURE);

    /* fill positions array with 1‑based indices */
    size_t pindex = 0;
    uniq = 0;  /* reuse variable to walk unique sums */
    for (size_t k = 0; k < sum_cap; ++k) {
        if (k == 0 || sums[k] != sums[k - 1]) {
            if ((sums[k] & 1ULL) == 0) {
                newpos[pindex++] = (seq_t)(uniq + 1);  /* 1‑based position */
            }
            uniq++;
        }
    }

    /* assign new data to outputs */
    *pos_out      = newpos;
    *pos_len_out  = pindex;
    *pos_cap_out  = even_cnt;
    *cur_limit_out = limit;

    free(sums);
}

/* Public entry point: returns the n‑th term of A024510 (1‑based). */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *positions      = NULL;
    static size_t positions_len = 0;
    static size_t positions_cap = 0;
    static idx_t  current_limit  = 0;
    static bool   initialized    = false;

    /* initialise with a first block (limit 1000).  Subsequent requests
     * will increase the limit in increments of 1000 until enough
     * entries are available. */
    if (!initialized) {
        recompute(1000, &positions, &positions_len,
                  &positions_cap, &current_limit);
        initialized = true;
    }

    /* grow table if necessary */
    while (n > positions_len) {
        recompute(current_limit + 1000, &positions, &positions_len,
                  &positions_cap, &current_limit);
    }

    /* n == 0 is undefined; return 0 as a safe sentinel */
    if (n == 0) return 0;

    return positions[n - 1];
}
