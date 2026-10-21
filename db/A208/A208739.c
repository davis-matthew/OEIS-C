/*
 * A208739  2^n minus the number of partitions of n.
 * https://oeis.org/A208739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - p(n) (where p(n) is the partition number of n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

/* Global storage for partition numbers */
static seq_t *partitions = NULL;   /* array of p(0)..p(m) */
static size_t part_len = 0;        /* number of entries currently defined (exclusive) */
static size_t part_cap = 0;        /* current allocation size */

/* Ensure that p(0)..p(n) are computed. */
static void ensure_partitions(idx_t n)
{
    /* Grow the array if necessary */
    if (n < part_len) return;  /* already ready */

    size_t new_cap = part_cap ? part_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    if (partitions == NULL) {
        partitions = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
    } else {
        partitions = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
    }
    part_cap = new_cap;

    /* initialise p(0) if this is the first time */
    if (part_len == 0) {
        partitions[0] = 1;
        part_len = 1;
    }

    /* Compute missing partition values using the pentagonal recurrence */
    for (idx_t i = part_len; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = partitions[i - pent1];
            if (k & 1) pos += term; else neg += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = partitions[i - pent2];
            if (k & 1) pos += term; else neg += term;
        }
        partitions[i] = pos - neg;
    }

    part_len = n + 1;   /* all indices up to n are now filled */
}

seq_t a(idx_t n)
{
    /* Make sure partition numbers up to n exist */
    ensure_partitions(n);

    /* Return 2^n - p(n) */
    return pow2(n) - partitions[n];
}
