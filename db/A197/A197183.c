/*
 * A197183  Number of partitions of n^2 into distinct factorials.
 * https://oeis.org/A197183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n^2 as a sum of distinct
 *            factorials. Implementation enumerates all subsets of 1! … 11!,
 *            which is sufficient for n ≤ 10 000 since 12! > 10 000². The
 *            subset sums are pre‑computed once and searched for each call to
 *            a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_ull.h"

#define FAC_CNT 11                    /* 1! … 11! are enough for n up to 10000 */

/* Static storage for all subset data (computed once) */
static unsigned long long factorials[FAC_CNT];
static unsigned long long subset_sums[1 << FAC_CNT];
static unsigned long long uniq_sums[1 << FAC_CNT];
static unsigned int uniq_counts[1 << FAC_CNT];
static size_t uniq_len = 0;
static bool init_flag = false;

/* Comparator for sorting unsigned long long values */

/* Pre‑compute factorials, all subset sums, and the unique sums with counts */
static void init_seq(void)
{
    /* 1! … 11! */
    unsigned long long f = 1;
    for (idx_t i = 1; i <= FAC_CNT; ++i) {
        f *= (unsigned long long)i;
        factorials[i - 1] = f;
    }

    /* All 2^11 subset sums */
    const size_t total_masks = 1u << FAC_CNT;
    for (size_t mask = 0; mask < total_masks; ++mask) {
        unsigned long long sum = 0;
        for (size_t j = 0; j < FAC_CNT; ++j)
            if (mask & (1u << j))
                sum += factorials[j];
        subset_sums[mask] = sum;
    }

    /* Sort the sums */
    qsort(subset_sums, total_masks, sizeof(unsigned long long), cmp_ull);

    /* Build unique sums and counts */
    if (total_masks > 0) {
        unsigned long long cur = subset_sums[0];
        unsigned int cnt = 1;
        for (size_t i = 1; i < total_masks; ++i) {
            if (subset_sums[i] == cur)
                ++cnt;
            else {
                uniq_sums[uniq_len]   = cur;
                uniq_counts[uniq_len] = cnt;
                ++uniq_len;
                cur = subset_sums[i];
                cnt = 1;
            }
        }
        uniq_sums[uniq_len]   = cur;
        uniq_counts[uniq_len] = cnt;
        ++uniq_len;
    }

    init_flag = true;
}

/* Public function defined by the OEIS format */
seq_t a(idx_t n)
{
    if (!init_flag)
        init_seq();

    /* Compute n^2 in a type that can hold the value */
    unsigned long long target = (unsigned long long)n * (unsigned long long)n;

    /* Binary search over the sorted unique sums */
    size_t lo = 0;
    size_t hi = uniq_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (uniq_sums[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }

    if (lo < uniq_len && uniq_sums[lo] == target)
        return (seq_t)uniq_counts[lo];
    return (seq_t)0;
}
