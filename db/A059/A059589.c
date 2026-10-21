/*
 * A059589  Sums of factorials of distinct integers.
 * https://oeis.org/A059589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3425, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_factorials
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059589: Sums of factorials of distinct integers.
   0! and 1! are treated as distinct elements.
   a(n) returns the n‑th smallest positive integer that can be
   represented as a sum of distinct factorials (counting 0! and 1!
   separately). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_factorials.h"

/* Comparison function for qsort (works for any integer‑like seq_t). */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Static storage for the sorted, unique sums. */
static bool       initialized = false;
static seq_t     *uniqs       = NULL;
static size_t      uniq_cnt   = 0;

/* Build the list of all distinct sums of distinct factorials
   using the first K factorials (indices 0 … K-1). */
static void build(void)
{
    /* 16 factorials (0! … 15!) are enough to produce > 10000 distinct sums. */
    const size_t K = 16;
    seq_t fact[K];
    compute_factorials(fact, K);

    /* All non‑empty subsets: 1 .. (2^K-1). */
    const size_t total_masks = ((size_t)1 << K) - 1;
    seq_t *tmp = (seq_t *)malloc(total_masks * sizeof(seq_t));
    if (!tmp) abort();

    /* Compute the sum for each subset. */
    size_t idx = 0;
    for (size_t mask = 1; mask < ((size_t)1 << K); ++mask) {
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < K; ++i)
            if (mask & ((size_t)1 << i))
                sum += fact[i];
        tmp[idx++] = sum;
    }

    /* Sort the sums.  We will deduplicate in place later. */
    qsort(tmp, total_masks, sizeof(seq_t), cmp_seq_t);

    /* Count unique values. */
    size_t uniq_local_cnt = 0;
    seq_t  prev = tmp[0];
    for (size_t i = 1; i < total_masks; ++i) {
        if (tmp[i] != prev) {
            ++uniq_local_cnt;
            prev = tmp[i];
        }
    }
    uniq_local_cnt += 1; /* include the first value */

    /* Allocate the array for unique sums. */
    uniqs = (seq_t *)malloc(uniq_local_cnt * sizeof(seq_t));
    if (!uniqs) abort();

    /* Copy unique values into uniqs. */
    uniqs[0] = tmp[0];
    size_t pos = 1;
    prev = tmp[0];
    for (size_t i = 1; i < total_masks; ++i) {
        if (tmp[i] != prev) {
            uniqs[pos++] = tmp[i];
            prev = tmp[i];
        }
    }
    uniq_cnt = uniq_local_cnt;

    free(tmp);
    initialized = true;
}

/* Return the n-th term (1‑based index).  Behaviour for n < 1 or beyond
   the computed range yields 0 (as the sequence is undefined there). */
seq_t a(idx_t n)
{
    if (!initialized)
        build();

    if ((size_t)n == 0 || (size_t)n > uniq_cnt)
        return (seq_t)0;

    return uniqs[(size_t)n - 1];
}
