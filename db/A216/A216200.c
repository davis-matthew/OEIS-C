/*
 * A216200  Number of disjoint trees that appear while iterating the sum of
 *          divisors function up to n.
 * https://oeis.org/A216200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 786, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A216200: Number of disjoint trees that appear while iterating the
 * sum of divisors function up to n.
 *
 * a(1) = 1
 * for n > 1: a(n) = a(n-1) + 1 - A054973(n)
 * A054973(n) counts the number of predecessor trees that are merged
 * when n is added.
 * The implementation builds the predecessor lists via a sieve,
 * then processes the numbers increasingly, maintaining a disjoint-set
 * union‑find structure to count the current number of trees.
 */

#include <stdlib.h>
#include <stdint.h>

/* Types seq_t and idx_t are supplied by the build environment. */

static idx_t max_n = 0;
static uint32_t *sigma_arr   = NULL;   /* temporary during rebuild */
static idx_t    *parent      = NULL;   /* DSU parent array */
static uint32_t *offsets     = NULL;   /* start index of predecessor list for each n */
static uint32_t *bucket_len  = NULL;   /* number of predecessors for each n */
static uint32_t *pred_list   = NULL;   /* concatenated predecessor lists */
static seq_t    *seq_vals    = NULL;   /* a(n) values */

/* Find with path compression */
static inline idx_t dsu_find(idx_t x, idx_t *pr)
{
    while (pr[x] != x) {
        pr[x] = pr[pr[x]];
        x = pr[x];
    }
    return x;
}

/* Rebuild all data structures up to new_limit (inclusive). */
static void rebuild(idx_t new_limit)
{
    /* Free old data */
    free(sigma_arr);
    free(parent);
    free(offsets);
    free(bucket_len);
    free(pred_list);
    free(seq_vals);

    max_n = new_limit;
    size_t sz = (size_t)(max_n + 1);

    sigma_arr  = (uint32_t *)calloc(sz, sizeof(uint32_t));
    parent     = (idx_t    *)calloc(sz, sizeof(idx_t));
    offsets    = (uint32_t *)calloc(sz, sizeof(uint32_t));
    bucket_len = (uint32_t *)calloc(sz, sizeof(uint32_t));
    seq_vals   = (seq_t    *)calloc(sz, sizeof(seq_t));

    /* Compute sigma(1..max_n) using a divisor sieve */
    for (idx_t d = 1; d <= max_n; ++d) {
        for (idx_t j = d; j <= max_n; j += d) {
            sigma_arr[(size_t)j] += (uint32_t)d;
        }
    }

    /* Count predecessors for each target value */
    for (idx_t i = 1; i <= max_n; ++i) {
        uint32_t k = sigma_arr[(size_t)i];
        if (k <= max_n) {
            bucket_len[(size_t)k] += 1U;
        }
    }

    /* Compute offsets – starting index of list for each m;
     * offsets[0] = 0; offsets[m] = sum_{t=0}^{m-1} bucket_len[t] */
    offsets[0] = 0U;
    for (idx_t m = 1; m <= max_n; ++m) {
        offsets[(size_t)m] = offsets[(size_t)(m-1)] + bucket_len[(size_t)(m-1)];
    }
    /* Total number of predecessor entries */
    size_t total_preds = (size_t)offsets[(size_t)max_n] + (size_t)bucket_len[(size_t)max_n];
    pred_list = (uint32_t *)malloc(total_preds * sizeof(uint32_t));

    /* Fill predecessor lists */
    uint32_t *temp_cnt = (uint32_t *)calloc(sz, sizeof(uint32_t));
    for (idx_t i = 1; i <= max_n; ++i) {
        uint32_t k = sigma_arr[(size_t)i];
        if (k <= max_n) {
            size_t pos = offsets[(size_t)k] + temp_cnt[(size_t)k];
            pred_list[pos] = (uint32_t)i;
            temp_cnt[(size_t)k] += 1U;
        }
    }
    free(temp_cnt);

    /* DSU initialization and computation of a(n) */
    idx_t root_count = 0;
    for (idx_t m = 1; m <= max_n; ++m) {
        parent[(size_t)m] = m;           /* new component for m */
        root_count += 1;

        idx_t start = offsets[(size_t)m];
        idx_t len   = bucket_len[(size_t)m];
        for (idx_t i = 0; i < len; ++i) {
            idx_t p   = pred_list[(size_t)(start + i)];
            idx_t rp  = dsu_find(p, parent);
            idx_t rm  = dsu_find(m, parent);  /* rm == m */
            if (rp != rm) {
                parent[(size_t)rp] = rm;
                root_count -= 1;
            }
        }
        seq_vals[(size_t)m] = (seq_t)root_count;
    }

    /* sigma no longer needed */
    free(sigma_arr);
    sigma_arr = NULL;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined, return 0 */
    if (n > max_n) rebuild(n);                   /* extend precomputation */
    return seq_vals[(size_t)n];
}
