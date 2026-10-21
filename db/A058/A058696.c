/*
 * A058696  Number of ways to partition 2n into positive integers.
 * https://oeis.org/A058696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(2*n) where p(k) is the partition number of k
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 63, u64 n <= 208, u128 n <= 729,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *part = NULL;          /* array of partition numbers */
static size_t part_capacity = 0;    /* allocated array size */
static idx_t computed_max = 0;      /* largest index whose value is known */

/* Compute all partition numbers up to max_val (inclusive). */
static void compute_partitions(idx_t max_val)
{
    /* Grow the array if necessary. */
    if ((size_t)(max_val + 1) > part_capacity) {
        size_t old_capacity = part_capacity;
        size_t new_size = (size_t)(max_val + 1);
        seq_t *new_part = alloc_seq_array(new_size);

        if (part) {
            for (size_t i = 0; i < old_capacity; ++i)
                new_part[i] = part[i];
            free_seq_array(part);
        }

        /* Zero‑initialize the new slots. */
        for (size_t i = old_capacity; i < new_size; ++i)
            new_part[i] = (seq_t)0;

        part = new_part;
        part_capacity = new_size;
    }

    /* Initialise p(0) if this is the first calculation. */
    if (computed_max == 0 && part_capacity > 0)
        part[0] = (seq_t)1;

    /* Compute partition numbers incrementally. */
    for (idx_t i = computed_max + 1; i <= max_val; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)
                break;
            seq_t term = part[i - g1];
            if (k & 1)
                sum += term;
            else
                sum -= term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i)
                continue;
            term = part[i - g2];
            if (k & 1)
                sum += term;
            else
                sum -= term;
        }
        part[i] = sum;
    }
    computed_max = max_val;
}

seq_t a(idx_t n)
{
    idx_t m = n << 1;          /* m = 2*n */
    compute_partitions(m);
    return part[m];
}
