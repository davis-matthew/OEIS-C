/*
 * A198194  Greatest number k such that p(k) <= n, p(k) being the number of
 *          unrestricted partitions of k.
 * https://oeis.org/A198194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest k such that p(k) <= n, where p(k) is the number
 *            of unrestricted partitions of k
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declarations of helper functions */
static void ensure_part_capacity(size_t needed);
static void compute_partitions_to(size_t k_target);

/* Static storage for partition numbers */
static seq_t *part          = NULL;   /* array of p(k) values, indices 0..max_k */
static size_t part_cap     = 0;       /* allocated capacity */
static size_t part_len     = 0;       /* current number of entries (max_k+1) */

/* Ensure that 'part' has capacity for at least 'needed' entries. */
static void ensure_part_capacity(size_t needed)
{
    if (needed <= part_cap)
        return;
    size_t new_cap = part_cap ? part_cap * 2 : 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(part, new_cap * sizeof(seq_t));
    part = tmp;
    part_cap = new_cap;
}

/* Compute all partition numbers p(0)..p(k_target) using the classic
   O(k^2) dynamic‑programming algorithm (G.F. approach). */
static void compute_partitions_to(size_t k_target)
{
    ensure_part_capacity(k_target + 1);

    /* Temporary array for intermediate DP values */
    seq_t *dp = (seq_t *)calloc(k_target + 1, sizeof(seq_t));
    dp[0] = 1;

    for (size_t i = 1; i <= k_target; ++i) {
        for (size_t j = i; j <= k_target; ++j) {
            dp[j] += dp[j - i];
        }
    }

    /* Copy results into the static array */
    for (size_t i = 0; i <= k_target; ++i)
        part[i] = dp[i];

    part_len = k_target + 1;
    free(dp);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise static array on first call */
    if (part == NULL) {
        ensure_part_capacity(2);
        part[0] = 1;          /* p(0) = 1 */
        part_len = 1;         /* only index 0 stored so far */
    }

    seq_t n_val = (seq_t)n;

    /* Extend the partition table until the last value exceeds n. */
    while (part_len == 0 || part[part_len - 1] <= n_val) {
        size_t next_k = part_len;          /* next k to compute (current max_k + 1) */
        compute_partitions_to(next_k);
    }

    /* part[part_len-1] > n, so the required k is part_len-2 */
    return (seq_t)(part_len - 2);
}
