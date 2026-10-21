/*
 * A222736  Total sum of parts of multiplicity 8 in all partitions of n.
 * https://oeis.org/A222736
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 139, u64 n <= 419,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_partitions, free_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k≥1, 8k≤n} k * (p(n-8k) - p(n-9k))
 * where p(m) is the number of unrestricted partitions of m.
 *
 * The implementation computes partition numbers up to the
 * largest requested n, stores them in a static array, and
 * then evaluates the above sum.  It works for any integer type
 * provided for seq_t and idx_t by the build system and is
 * valid in C11 and C++17.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uint64_t */
#include "../utils/alloc_partitions.h"
#include "../utils/free_partitions.h"

static seq_t *partitions = NULL;     /* array of partition numbers */
static size_t part_capacity = 0;     /* allocated size of array */
static idx_t  part_max_n   = 0;      /* largest n for which partitions[0..n] computed */

/* Recompute all partition numbers up to N (inclusive). */
static void recompute_partitions(idx_t N)
{
    size_t new_cap = (size_t)N + 1;
    /* Allocate new array */
    seq_t *newp = alloc_partitions(new_cap);
    newp[0] = (seq_t)1;                 /* p(0) = 1 */

    /* Classic DP: for each part size i, add its contributions. */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; ++j) {
            newp[j] += newp[j - i];
        }
    }

    /* Replace old array */
    free_partitions(partitions);
    partitions     = newp;
    part_capacity  = new_cap;
    part_max_n     = N;
}

/* Public function computing the sequence term. */
seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0;   /* offset is 8 */

    /* Ensure partition numbers up to n are available */
    if (n > part_max_n) {
        if (partitions == NULL) {
            recompute_partitions(n);
        } else {
            /* For efficiency, recompute only up to the new maximum. */
            recompute_partitions(n);
        }
    }

    seq_t result = (seq_t)0;

    for (idx_t k = 1; 8*k <= n; ++k) {
        idx_t m1 = n - 8 * k;
        idx_t m2 = n - 9 * k;

        seq_t term = partitions[m1];
        if (m2 <= n) term -= partitions[m2];

        result += (seq_t)k * term;
    }

    return result;
}
