/*
 * A210765  Triangle read by rows in which row n lists the number of
 *          partitions of n together with n-1 ones.
 * https://oeis.org/A210765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A210765: Triangle read by rows: each row r contains p(r) followed by r-1 ones.
 * a(n) is the n-th element of the flattened triangle.
 * Formula implemented:
 *   Find row r such that r*(r+1)/2 ≥ n.
 *   Let k = n - r*(r-1)/2.
 *   If k == 1  → a(n) = p(r) (partition number of r)
 *   else       → a(n) = 1
 *
 * Partition numbers are computed on demand using the pentagonal number theorem.
 * The implementation caches computed values for efficiency.
 */

#include <stdlib.h>

/* partition_cache holds p(0) … p(max_cached). */
static seq_t *partition_cache = NULL;
static idx_t cache_size = 0;

/* Compute the smallest r such that r*(r+1)/2 ≥ n. */
static idx_t find_row(idx_t n) {
    idx_t low = 1, high = 1;
    /* Find an upper bound by doubling. */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* Binary search. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Ensure the partition cache contains values up to r. */
static void ensure_partition_cache(idx_t r) {
    if (r < cache_size)
        return;
    /* Allocate / enlarge cache. */
    partition_cache = (seq_t *)realloc(partition_cache, (r + 1) * sizeof(seq_t));
    if (cache_size == 0)
        partition_cache[0] = (seq_t)1, cache_size = 1;
    /* Compute missing partitions using the pentagonal theorem. */
    for (idx_t n = cache_size; n <= r; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            if (k & 1) pos += partition_cache[n - pent1];
            else      neg += partition_cache[n - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) continue;
            if (k & 1) pos += partition_cache[n - pent2];
            else      neg += partition_cache[n - pent2];
        }
        partition_cache[n] = pos - neg;
    }
    cache_size = r + 1;
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;   /* Undefined for n < 1, return 0 as safe default. */

    /* Determine row and position inside the row. */
    idx_t r = find_row(n);
    idx_t prev_sum = (unsigned __int128)(r - 1) * r / 2;  /* Sum of first (r-1) rows */
    idx_t pos_in_row = n - prev_sum;                     /* 1-based position */

    if (pos_in_row == 1) {
        ensure_partition_cache(r);
        return partition_cache[r];
    }
    return (seq_t)1;
}
