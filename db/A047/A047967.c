/*
 * A047967  Number of partitions of n with some part repeated.
 * https://oeis.org/A047967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - q(n) where p(n) is the ordinary partition function
 *            (OEIS A000041) and q(n) is the number of partitions of n into
 *            distinct parts (OEIS A000009)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *partitions = NULL;   /* p[n] */
static seq_t *dist_parts = NULL;   /* q[n] */
static idx_t    capacity  = 0;     /* current array size */

/* compute partitions and distinct-partitions up to at least 'n' */
static void compute_upto(idx_t n)
{
    idx_t new_cap = capacity;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= n) new_cap <<= 1;          /* grow in powers of two */

    partitions = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
    dist_parts = (seq_t *)realloc(dist_parts,   new_cap * sizeof(seq_t));

    for (idx_t i = 0; i < new_cap; ++i) {
        partitions[i] = 0;
        dist_parts[i] = 0;
    }
    partitions[0] = 1;   /* one way to partition 0 */
    dist_parts[0] = 1;

    /* ordinary partitions – unlimited multiplicity (unbounded knapsack) */
    for (idx_t k = 1; k < new_cap; ++k) {
        for (idx_t i = k; i < new_cap; ++i) {
            partitions[i] += partitions[i - k];
        }
    }

    /* partitions into distinct parts – 0/1 knapsack */
    for (idx_t k = 1; k < new_cap; ++k) {
        for (idx_t i = new_cap; i-- > k; ) {
            dist_parts[i] += dist_parts[i - k];
        }
    }

    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= capacity) compute_upto(n);
    return partitions[n] - dist_parts[n];
}
