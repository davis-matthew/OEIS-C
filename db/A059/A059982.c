/*
 * A059982  Symmetric array of numeric partitions related to 1 4 9 16 ... and
 *          1 3 4 7 13 ..., read by rows.
 * https://oeis.org/A059982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 31, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059982: symmetric array of numeric partitions.
   The array is generated row by row.  Row r contains 2·⌊√r⌋+1 columns.
   At rows r that are perfect squares (r = k²) two new columns are
   inserted, one on each side.  Values in each column are partition
   numbers: in column that originated at row s = i² the j‑th entry
   (row j ≥ s) equals p(j‑s).  The sequence is the row‑wise
   concatenation of the array.  This implementation follows the
   description and memoises partition numbers up to the largest value
   required.  It works for any compatible seq_t (including an
   arbitrary‑precision class) and any idx_t that is an unsigned 64‑bit
   integer.  No external libraries other than the listed utils are used. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Integer square root: floor(sqrt(n)) for idx_t */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            if ((mid + 1) > 0 && (mid + 1) > n / (mid + 1)) {
                return mid;
            }
            low = mid + 1;
        }
    }
    return low;   /* should never be reached */
}

/* Cached partition numbers: p(0) = 1 */
static seq_t *partition_cache = NULL;
static size_t  cache_cap     = 0;

/* Ensure partition numbers are available up to 'limit' (inclusive). */
static void ensure_partitions(size_t limit)
{
    if (limit + 1 <= cache_cap) return;          /* already enough */

    seq_t *new_cache = seq_alloc_new_array(limit + 1);
    if (!new_cache) abort();

    new_cache[0] = 1;                            /* p(0) = 1 */

    /* Dynamic programming: p(n) = Σ_{i=1..n} p(n-i) (unrestricted partitions). */
    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; ++j) {
            new_cache[j] += new_cache[j - i];
        }
    }

    seq_free_array(partition_cache);
    partition_cache = new_cache;
    cache_cap       = limit + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row that contains index n. */
    idx_t sum = 0;          /* total elements before the current row */
    idx_t r   = 0;          /* current row number */

    while (1) {
        idx_t k = isqrt_idx(r);          /* floor(sqrt(r)) */
        idx_t width = 2 * k + 1;         /* number of columns in row r */
        if (sum + width > n) break;
        sum += width;
        ++r;
    }

    /* Position within the found row. */
    idx_t pos = n - sum;         /* 0 <= pos < width(r) */
    idx_t k   = isqrt_idx(r);    /* same k as above */

    /* Compute distance i from the centre column. */
    idx_t i;
    if (pos == k) {
        i = 0;                   /* centre column, s = 0 */
    } else if (pos < k) {
        i = k - pos;              /* left side column */
    } else {
        i = pos - k;              /* right side column */
    }

    /* The column originates from row s = i² → value = p(r - i²) */
    idx_t sub = r - i * i;
    size_t needed = (size_t)sub;           /* index for the partition table */

    ensure_partitions(needed);
    return partition_cache[needed];
}
