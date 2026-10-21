/*
 * A083531  First difference sequence of A002191. Differences between possible
 *          values for sum of divisors of n.
 * https://oeis.org/A083531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between successive distinct values of the
 *            sum‐of‐divisors function σ(n) (OEIS A002191). The sequence is
 *            obtained by first generating all σ values up to a sufficiently
 *            large bound, extracting the distinct values, sorting them, and
 *            then taking successive differences. The implement...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper comparison for qsort (works for integral types and uint128). */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Static state to cache results. */
static seq_t *sigma   = NULL; /* |sigma| = M+1 */
static seq_t *diffs   = NULL; /* |diffs| = number of distinct σ minus 1 */
static size_t M       = 500000;   /* bound: try to get enough distinct values */
static size_t sigma_len = 0;
static size_t diffs_len = 0;
static bool ready = false;
static idx_t computed_max_n = 0;

/* Initialise or enlarge the precomputation if necessary. */
static void init_sequence(idx_t n)
{
    if (ready && n <= (idx_t)computed_max_n)
        return;                     /* already enough */

    /* Allocate sigma array. */
    sigma_len = M + 1;
    sigma = (seq_t *)malloc(sigma_len * sizeof(seq_t));
    if (!sigma) return;   /* out of memory – will return 0 */
    for (size_t i = 0; i < sigma_len; ++i) sigma[i] = 0;

    /* Sieve to compute σ(i) for 1 <= i <= M. */
    for (size_t i = 1; i <= M; ++i) {
        seq_t val = (seq_t)i;
        for (size_t j = i; j <= M; j += i) {
            sigma[j] += val;
        }
    }

    /* Collect all σ values into a temporary array. */
    size_t total = 0;
    for (size_t i = 1; i <= M; ++i) ++total;
    seq_t *all = (seq_t *)malloc(total * sizeof(seq_t));
    if (!all) {
        free(sigma);
        sigma = NULL;
        return;
    }
    size_t idx = 0;
    for (size_t i = 1; i <= M; ++i) {
        all[idx++] = sigma[i];
    }

    /* Sort the array and deduplicate. */
    qsort(all, total, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    if (total > 0) {
        all[uniq++] = all[0];
    }
    for (size_t i = 1; i < total; ++i) {
        if (all[i] != all[i - 1]) {
            all[uniq++] = all[i];
        }
    }

    /* Compute successive differences. */
    if (uniq <= 1) {
        diffs_len = 0;
        diffs = NULL;
    } else {
        diffs_len = uniq - 1;
        diffs = (seq_t *)malloc(diffs_len * sizeof(seq_t));
        if (!diffs) {
            free(all);
            free(sigma);
            sigma = NULL;
            return;
        }
        for (size_t i = 0; i < diffs_len; ++i) {
            diffs[i] = all[i + 1] - all[i];
        }
    }

    /* Cleanup temporary array and set flags. */
    free(all);
    free(sigma);
    sigma = NULL;
    ready = true;
    computed_max_n = (idx_t)diffs_len;
    /* If requested n is larger than what we computed, we would need to
       increase M.  For the problem constraints (n <= 10000) M = 500000
       provides considerably more distinct values than needed. */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined, but return 0 for safety */
    init_sequence(n);
    if (n <= (idx_t)diffs_len) {
        return diffs[n - 1];
    } else {
        return 0;  /* out of range, should not happen for given constraints */
    }
}
