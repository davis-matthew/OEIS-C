/*
 * A182702  Triangle T(n,k) = n*(A000041(n-k)) read by rows, k>=0.
 * https://oeis.org/A182702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T(r,k) = r *
 *            A000041(r-k) read row‑wise. For an index n (n≥1) the row r
 *            satisfies r(r-1)/2 < n ≤ r(r+1)/2 and the column index is k = n
 *            – r(r-1)/2 – 1. The value is a(n) = r * A000041(r-k)
 * Range:     u8 n <= 36, u16 n <= 351, u32 n <= 3916, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/*   Partition numbers  p(0)=1,  p(m)=∑_{k≠0} (-1)^{k+1} p(m- k(3k±1)/2)  */
/* ------------------------------------------------------------------------- */
static seq_t *part_cache = NULL;
static idx_t part_cache_max = 0;          /* largest m with part_cache[m] computed */

/* Ensure that part_cache[0..max_needed] is available. */
static void ensure_partition(idx_t max_needed)
{
    if (max_needed <= part_cache_max)
        return;

    seq_t *tmp = (seq_t *)realloc(part_cache,
                                 (max_needed + 1) * sizeof(seq_t));
    if (!tmp)
        abort();
    part_cache = tmp;

    if (part_cache_max == 0) {           /* first time: set p(0)=1 */
        part_cache[0] = (seq_t)1;
        part_cache_max = 0;
    }

    for (idx_t m = part_cache_max + 1; m <= max_needed; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m)
                break;
            if ((k & 1ULL) == 1)
                sum += part_cache[m - pent1];
            else
                sum -= part_cache[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m)
                continue;
            if ((k & 1ULL) == 1)
                sum += part_cache[m - pent2];
            else
                sum -= part_cache[m - pent2];
        }
        part_cache[m] = sum;
    }
    part_cache_max = max_needed;
}

/* ------------------------------------------------------------------------- */
/*   Main sequence function                                               */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n≥1 in OEIS */
        return (seq_t)0;

    /* Find row r such that r(r-1)/2 < n ≤ r(r+1)/2.  */
    idx_t low = 1, high = 1;
    while ((high * (high + 1)) / 2 < n)
        high *= 2;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;

    /* Previous triangular count. */
    idx_t prev = (row - 1) * row / 2;

    /* Zero‑based column index within the row. */
    idx_t k = n - prev - 1;

    /* Index for the partition number (m = row - k). */
    idx_t part_idx = row - k;
    if (part_idx > part_cache_max)
        ensure_partition(part_idx);

    return ((seq_t)row) * part_cache[part_idx];
}
