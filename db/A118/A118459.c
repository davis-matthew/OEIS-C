/*
 * A118459  Lengths of partitions into distinct parts in Mathematica order.
 * https://oeis.org/A118459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating partitions of integers into distinct parts
   in Mathematica's default order (partitions with parts sorted in
   decreasing order and the list of partitions sorted lexicographically
   by that decreasing sequence). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global tables for partition counts and cumulative counts.
   counts[r * (S+1) + m] = number of partitions of r
   with parts <= m (distinct parts). */
static idx_t *counts = NULL;      /* 1D array, size (S+1)*(S+1)   */
static idx_t *cum = NULL;         /* 1D array, size S+1          */
static idx_t currentMaxS = 0;     /* largest sum for which tables are built */
static idx_t cum_total = 0;       /* cum[currentMaxS], total partitions */

/* Rebuild tables for sums up to newS (inclusive). Assumes newS > currentMaxS. */
static void rebuild_tables(idx_t newS)
{
    size_t rows = (size_t)(newS + 1);
    size_t newSize = rows * rows;

    /* Allocate and compute new counts table */
    idx_t *new_counts = (idx_t *)calloc(newSize, sizeof(idx_t));
    if (!new_counts) exit(EXIT_FAILURE);

    /* Fill counts using recurrence */
    for (idx_t m = 0; m <= newS; ++m) {
        for (idx_t r = 0; r <= newS; ++r) {
            size_t idx = (size_t)r * rows + (size_t)m;
            if (r == 0) {
                new_counts[idx] = 1;            /* empty partition */
            } else if (m == 0) {
                new_counts[idx] = 0;            /* cannot partition positive r with max part 0 */
            } else {
                idx_t without_m = new_counts[(size_t)r * rows + (size_t)(m - 1)];
                idx_t with_m = 0;
                if (r >= m) {
                    with_m = new_counts[(size_t)(r - m) * rows + (size_t)(m - 1)];
                }
                new_counts[idx] = without_m + with_m;
            }
        }
    }

    /* Allocate and compute new cumulative table */
    size_t cumSize = (size_t)(newS + 1);
    idx_t *new_cum = (idx_t *)malloc(cumSize * sizeof(idx_t));
    if (!new_cum) exit(EXIT_FAILURE);

    new_cum[0] = 1;  /* only the empty partition for sum 0 */
    for (idx_t s = 1; s <= newS; ++s) {
        /* partitions of sum s are counts[s][s] (max part <= s) */
        idx_t partCount = new_counts[(size_t)s * rows + (size_t)s];
        new_cum[s] = new_cum[s - 1] + partCount;
    }

    /* Replace old tables */
    free(counts);
    free(cum);
    counts = new_counts;
    cum    = new_cum;
    currentMaxS = newS;
    cum_total = cum[currentMaxS];
}

/* Ensure tables cover at least n+1 partitions. */
static void ensure_capacity(idx_t n)
{
    while (cum_total <= n) {
        rebuild_tables(currentMaxS + 1);
    }
}

/* Find the length of the offset‑th partition (0‑based) of 'sum' in lexicographic
   descending order (Mathematica order). */
static uint32_t partition_length(idx_t sum, idx_t offset)
{
    uint32_t len = 0;
    idx_t remaining = sum;
    idx_t maxPart = sum;

    while (remaining > 0) {
        for (idx_t p = remaining < maxPart ? remaining : maxPart; p >= 1; --p) {
            idx_t cnt = counts[(size_t)(remaining - p) * (currentMaxS + 1) + (size_t)(p - 1)];
            if (offset < cnt) {
                /* choose part p */
                ++len;
                remaining -= p;
                maxPart = p - 1;
                break;
            } else {
                offset -= cnt;
            }
        }
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    ensure_capacity(n);

    /* Binary search for S: cum[S-1] <= n < cum[S] */
    idx_t low = 0, high = currentMaxS;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (cum[mid] <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t S = low;

    idx_t offset = n - cum[S - 1];          /* 0‑based offset within partitions of sum S */
    uint32_t len = partition_length(S, offset);
    return (seq_t)len;
}
