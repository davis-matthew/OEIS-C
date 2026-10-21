/*
 * A081094  4th differences of partition numbers A000041.
 * https://oeis.org/A081094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 37, i16 n <= 97, i32 n <= 231, i64 n <= 623,
 *            i128 n <= 1896, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081094 – 4th differences of partition numbers A000041
   a(n) = p(n+4) − 4 p(n+3) + 6 p(n+2) − 4 p(n+1) + p(n)
   (offset 0, a(0)=2)                                            */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* The types idx_t and seq_t are supplied by the build environment */
static seq_t *partitions = NULL;    /* partitions[0 … current_max]          */
static size_t part_capacity = 0;    /* allocated array size                  */
static idx_t part_max = 0;          /* largest index whose p[.] is computed */
static int part_initialized = 0;    /* flag for first allocation */

/* Ensure that partition numbers up to N (inclusive) are available.
   N is an unsigned 64‑bit index.  The function allocates/reallocates
   the array as necessary and fills any missing values using the
   recurrence with generalized pentagonal numbers. */
static void ensure_partitions(idx_t N)
{
    if (N <= part_max) return;

    /* Allocate or grow the array if required. */
    if (part_capacity == 0) {
        part_capacity = 64;
        partitions = (seq_t *)realloc(partitions, part_capacity * sizeof(seq_t));
        if (!partitions) return;      /* allocation failed – keep old data */
        /* initialise base case */
        partitions[0] = (seq_t)1;
        part_max = 0;
        part_initialized = 1;
    } else if (N + 1 > part_capacity) {
        size_t new_cap = part_capacity * 2;
        if (new_cap < (size_t)(N + 1)) new_cap = (size_t)(N + 1);
        seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed – keep old data */
        partitions = tmp;
        part_capacity = new_cap;
    }

    /* Compute missing partition numbers. */
    for (idx_t k = part_max + 1; k <= N; ++k) {
        seq_t sum = 0;
        for (idx_t r = 1;; ++r) {
            /* Generalised pentagonal numbers g1 = r(3r−1)/2, g2 = r(3r+1)/2 */
            idx_t g1 = r * (3 * r - 1) / 2;
            if (g1 > k) break;
            sum += (r & 1) ? partitions[k - g1] : -partitions[k - g1];

            idx_t g2 = r * (3 * r + 1) / 2;
            if (g2 > k) continue;
            sum += (r & 1) ? partitions[k - g2] : -partitions[k - g2];
        }
        partitions[k] = sum;
    }
    part_max = N;
}

/* Public function: a(n) for OEIS A081094. */
seq_t a(idx_t n)
{
    ensure_partitions(n + 4);

    seq_t res = partitions[n + 4];
    res -= partitions[n + 3] * (seq_t)4;
    res += partitions[n + 2] * (seq_t)6;
    res -= partitions[n + 1] * (seq_t)4;
    res += partitions[n];
    return res;
}
