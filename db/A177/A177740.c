/*
 * A177740  Triangle read by rows in which row n lists the number of parts in
 *          each of those partitions of n that have every part >=3.
 * https://oeis.org/A177740
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the number of parts of the k‑th partition of n (parts ≥
 *            3) when the partitions of each n are listed in descending
 *            lexicographic order. The function searches the triangular array
 *            defined by OEIS A177740
 * Range:     u8 n <= 700, u16 n <= 9549, u32 n <= 9549, u64 n <= 9549,
 *            u128 n <= 9549, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only standard headers that work in C11 and C++17. */
#include <stdint.h>
#include <stddef.h>

#ifndef LIMIT
#define LIMIT 600            /* sufficient for any reasonable index */
#endif

/* Partition counts and cumulative sums are pre‑computed once. */
static seq_t f[ LIMIT + 1 ][ LIMIT + 1 ];   /* f[s][m] = number of partitions of s
                                               with parts ≥3 and ≤m */
static seq_t partition_count[ LIMIT + 1 ];
static seq_t cum_sum[ LIMIT + 1 ];

/* Flag to initialise static tables only once. */
static int tables_initialised = 0;

static void initialise_tables(void)
{
    if (tables_initialised)
        return;
    tables_initialised = 1;

    /* f[0][m] = 1 for all m (empty partition). */
    for (int m = 0; m <= LIMIT; ++m)
        f[0][m] = 1;

    /* For s>0 and m<3, no part can be chosen; f[s][m] = 0. */
    for (int s = 1; s <= LIMIT; ++s)
        for (int m = 0; m < 3; ++m)
            f[s][m] = 0;

    /* Compute f using the standard recurrence:
       f[s][m] = f[s][m-1] + (m≥3 && s≥m ? f[s-m][m] : 0). */
    for (int m = 3; m <= LIMIT; ++m) {
        for (int s = 1; s <= LIMIT; ++s) {
            f[s][m] = f[s][m - 1];
            if (s >= m)
                f[s][m] += f[s - m][m];
        }
    }

    /* partition_count[n] = f[n][n], and cum_sum. */
    partition_count[0] = partition_count[1] = partition_count[2] = 0;
    cum_sum[0] = cum_sum[1] = cum_sum[2] = 0;
    for (int n = 3; n <= LIMIT; ++n) {
        partition_count[n] = f[n][n];
        cum_sum[n] = cum_sum[n - 1] + partition_count[n];
    }
}

/* Recursive helper to find the number of parts in the k-th partition of 'sum'
   with parts ≥3 in descending lexicographic order. */
static seq_t kth_partition_parts(int sum, int max_part, seq_t k)
{
    /* sum==0 signals a valid end; no more parts to add. */
    if (sum == 0)
        return 0;

    for (int part = max_part; part >= 3; --part) {
        if (part > sum)
            continue;
        seq_t cnt = f[sum - part][part];   /* number of partitions of the residual */
        if (k > cnt) {
            k -= cnt;
            continue;
        }
        /* The k‑th partition starts with 'part'. */
        return 1 + kth_partition_parts(sum - part, part, k);
    }
    /* Should never reach here if k is valid. */
    return 0;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;          /* undefined for indices below the offset */

    initialise_tables();

    /* Convert linear index to the position inside the triangle. */
    seq_t idx_adj = (seq_t)n - 3;   /* zero‑based index within the flattened array */

    /* Find the row r such that cum_sum[r-1] <= idx_adj < cum_sum[r] */
    int r = 3;
    while (r <= LIMIT && cum_sum[r] <= idx_adj)
        ++r;

    /* In case we exceed pre‑computed LIMIT (very unlikely for the given constraints) */
    if (r > LIMIT)
        return 0;   /* safety fallback */

    /* Position k (1‑based) within row r */
    seq_t k = idx_adj - cum_sum[r - 1] + 1;

    /* Compute the number of parts in the k‑th partition of r */
    return kth_partition_parts(r, r, k);
}
