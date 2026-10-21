/*
 * A064173  Number of partitions of n with positive rank.
 * https://oeis.org/A064173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, n‑(3k²+k)/2 ≥ 0} (-1)^(k-1) • p(n‑(3k²+k)/2)
 *            where p(m) is the partition function A000041(m)
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 134, u64 n <= 428,
 *            u128 n <= 1479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamically cache partition numbers p(0)…p(maxN). */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t */

static seq_t *partitions = NULL; /* p[0..maxN] */
static idx_t  maxN = 0;          /* largest n for which partitions is valid */

/* Compute partition numbers up to 'up_to' (inclusive). */
static void ensure_partitions(idx_t up_to)
{
    if (up_to <= maxN)
        return;                     /* already computed */

    /* Allocate or grow the array */
    seq_t *newp = (seq_t *)realloc(partitions,
                                   (size_t)(up_to + 1) * sizeof(seq_t));
    if (!newp) {
        free(partitions);
        abort(); /* memory exhausted */
    }
    partitions = newp;

    /* Initialise to 1 if this is the first allocation */
    if (maxN == 0) {
        partitions[0] = 1;          /* p(0) = 1 */
    }

    /* Compute new partition values from maxN+1 to up_to. */
    for (idx_t n = maxN + 1; n <= up_to; ++n) {
        seq_t value = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n)
                break;
            seq_t sign = (k & 1) ? 1 : -1; /* +1 for odd k, -1 for even k */
            value += sign * partitions[n - pent1];
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n)
                continue;
            value += sign * partitions[n - pent2];
        }
        partitions[n] = value;
    }

    maxN = up_to;
}

/* Return a(n) for n ≥ 1.  Works for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* not defined for n < 1 */

    /* Make sure partition numbers up to n are available. */
    ensure_partitions(n);

    seq_t pos = 0, neg = 0;            /* sums of positive and negative terms */
    for (idx_t k = 1;; ++k) {
        idx_t pent = k * (3 * k + 1) / 2;   /* generalized pentagonal (3k²+k)/2 */
        if (pent > n)
            break;
        idx_t idx = n - pent;
        if (k & 1)                      /* k odd → positive sign */
            pos += partitions[idx];
        else                            /* k even → negative sign */
            neg += partitions[idx];
    }
    return pos - neg;                   /* pos ≥ neg for all n */
}
