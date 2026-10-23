/*
 * A199121  Number of partitions of n into distinct terms of (1,4)-Ulam
 *          sequence, cf. A003666.
 * https://oeis.org/A199121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A199121:
   Number of partitions of n into distinct terms of the (1,4)-Ulam sequence.
   Generate all Ulam terms ≤ n, then use a 0/1 knapsack DP to count
   partitions into distinct such terms. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Global state for the (1,4)-Ulam sequence */
static uint64_t *ulam = NULL;       /* array of Ulam terms                 */
static size_t ulam_len = 0;         /* number of terms stored              */
static size_t ulam_cap = 0;         /* allocated capacity of ulam array    */
static uint64_t ulam_max_val = 0;   /* largest term currently stored       */

/* Count of representations as sum of two distinct terms */
static uint16_t *sum_counts = NULL;
static size_t sum_cap = 0;

/* Ensure that the Ulam sequence contains all terms ≤ n. */
static void ensure_ulam(idx_t n)
{
    if (ulam_len == 0) {
        /* initial capacity */
        ulam_cap = 4;
        ulam = (uint64_t *)malloc(ulam_cap * sizeof(uint64_t));
        if (!ulam) return;
        ulam[0] = 1;
        ulam[1] = 4;
        ulam_len = 2;
        ulam_max_val = 4;

        /* sum_counts array for sums up to 2*n */
        sum_cap = 12;
        sum_counts = (uint16_t *)malloc(sum_cap * sizeof(uint16_t));
        if (!sum_counts) return;
        for (size_t i = 0; i < sum_cap; ++i) sum_counts[i] = 0;
        sum_counts[5] = 1;          /* 1 + 4 = 5 */
    }

    /* enlarge sum_counts if needed */
    size_t needed = (size_t)(2 * n + 10);
    if (needed > sum_cap) {
        size_t new_cap = needed;
        sum_counts = (uint16_t *)realloc(sum_counts, new_cap * sizeof(uint16_t));
        for (size_t i = sum_cap; i < new_cap; ++i) sum_counts[i] = 0;
        sum_cap = new_cap;
    }

    /* Generate new Ulam terms until ulam_max_val > n */
    while (ulam_max_val <= n) {
        uint64_t candidate = ulam_max_val + 1;

        /* Find next integer with exactly one representation */
        while (1) {
            if (candidate >= sum_cap) {
                size_t new_cap = candidate * 2 + 10;
                sum_counts = (uint16_t *)realloc(sum_counts,
                        new_cap * sizeof(uint16_t));
                for (size_t i = sum_cap; i < new_cap; ++i)
                    sum_counts[i] = 0;
                sum_cap = new_cap;
            }
            if (sum_counts[candidate] == 1) break;
            ++candidate;
        }

        /* Ensure capacity for ulam array */
        if (ulam_len == ulam_cap) {
            size_t new_cap = ulam_cap * 2 + 16;
            ulam = (uint64_t *)realloc(ulam,
                    new_cap * sizeof(uint64_t));
            ulam_cap = new_cap;
        }
        ulam[ulam_len++] = candidate;
        ulam_max_val = candidate;

        /* Update sums involving the new term */
        for (size_t i = 0; i < ulam_len - 1; ++i) {
            uint64_t s = ulam[i] + candidate;
            if (s >= sum_cap) {
                size_t new_cap = s * 2 + 10;
                sum_counts = (uint16_t *)realloc(sum_counts,
                        new_cap * sizeof(uint16_t));
                for (size_t j = sum_cap; j < new_cap; ++j)
                    sum_counts[j] = 0;
                sum_cap = new_cap;
            }
            sum_counts[s] += 1;
        }
    }
}

/* Global DP cache */
static seq_t *dp = NULL;
static size_t dp_cap = 0;

/* Recompute dp for 0..n using 0/1 knapsack over Ulam terms. */
static void recompute_dp(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (dp_cap < needed) {
        if (dp) seq_free_array(dp);
        dp = seq_alloc_array(needed);
        dp_cap = needed;
    }

    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (size_t k = 0; k < ulam_len; ++k) {
        uint64_t term_u64 = ulam[k];
        idx_t term = (idx_t)term_u64;
        if (term > n) break;
        for (idx_t s = n; s >= term; --s) {
            dp[s] += dp[s - term];
        }
    }
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    ensure_ulam(n);
    recompute_dp(n);
    return dp[n];
}
