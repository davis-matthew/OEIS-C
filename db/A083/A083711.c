/*
 * A083711  a(n) = A083710(n) - A000041(n-1).
 * https://oeis.org/A083711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n} A000041(d-1)
 * Range:     u8 n <= 35, u16 n <= 89, u32 n <= 257, u64 n <= 835,
 *            u128 n <= 2919, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where A000041 is the partition function, with A000041(0)=1) */
/* Special case: a(1) = 1 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"   /* provides ensure_capacity */

static seq_t *partitions = NULL;   /* partitions[k] = A000041(k) */
static size_t part_cap = 0;        /* allocated slots in partitions */
static idx_t  max_calc   = 0;      /* highest index computed so far */

/* compute A000041(k) for all k <= up_to */
static void compute_partitions_up_to(idx_t up_to)
{
    if (up_to <= max_calc) return;

    /* allocate enough space */
    ensure_capacity(up_to + 1, &part_cap, (void **)&partitions, sizeof(seq_t));

    /* initialise the single known value */
    if (max_calc == 0) {
        partitions[0] = 1;
        max_calc = 0;
    }

    for (idx_t n = max_calc + 1; n <= up_to; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            seq_t term = partitions[n - pent1];
            if (k & 1) sum += term; else sum -= term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= n) {
                term = partitions[n - pent2];
                if (k & 1) sum += term; else sum -= term;
            }
        }
        partitions[n] = sum;
    }
    max_calc = up_to;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* by definition */

    /* need partitions up to n-1 */
    compute_partitions_up_to(n - 1);

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 < n) result += partitions[d1 - 1];
            if (d2 != d1 && d2 < n) result += partitions[d2 - 1];
        }
    }
    return result;
}
