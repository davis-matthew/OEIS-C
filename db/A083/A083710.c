/*
 * A083710  Number of integer partitions of n with a part dividing all the
 *          other parts.
 * https://oeis.org/A083710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} p(d-1) where p(k) is the partition number of k
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 128, u64 n <= 417,
 *            u128 n <= 1459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* The build system supplies the types idx_t and seq_t.  This file should
 * compile with both C11 and C++17, so no language‑specific features are used.
 */

static seq_t *part = NULL;      /* part[i] = p(i) for 0 <= i <= part_nmax */
static idx_t part_nmax = 0;      /* maximum i for which part[i] is valid */

/* Ensure that the partition array has entries for all indices up to 'up_to'.
 * The calculation uses the recurrence with generalized pentagonal numbers. */
static void ensure_partitions(idx_t up_to)
{
    /* Initialise the array if it is still empty.  It must contain part[0] = 1. */
    if (part == NULL) {
        part = (seq_t *)malloc(sizeof(seq_t));
        if (!part) abort();
        part[0] = 1;
        part_nmax = 0;
    }

    if (up_to <= part_nmax) return;  /* already computed */

    /* Grow the array to hold indices 0 .. up_to. */
    size_t new_cap = (size_t)(up_to + 1);
    seq_t *new_part = (seq_t *)realloc(part, new_cap * sizeof(seq_t));
    if (!new_part) abort();
    part = new_part;

    /* Compute missing partition values. */
    for (idx_t m = part_nmax + 1; m <= up_to; m++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;      /* generalized pentagonal number */
            if (g1 > m) break;
            if (k & 1)
                pos += part[(size_t)(m - g1)];
            else
                neg += part[(size_t)(m - g1)];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            if (k & 1)
                pos += part[(size_t)(m - g2)];
            else
                neg += part[(size_t)(m - g2)];
        }
        part[m] = pos - neg;
    }

    part_nmax = up_to;
}

/* Compute A083710(n).  The function has external linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 by definition */

    /* We need partition numbers up to n-1. */
    if (n > 0) {
        ensure_partitions(n - 1);
    }

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            /* d is a divisor of n.  Add p(d-1). */
            result += part[(size_t)(d - 1)];

            idx_t d2 = n / d;
            if (d2 != d) {
                /* n/d is a different divisor. */
                result += part[(size_t)(d2 - 1)];
            }
        }
    }
    return result;
}
