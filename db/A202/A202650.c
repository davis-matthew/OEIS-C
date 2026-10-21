/*
 * A202650  Number of ways to write n = p + p(k) + p(m) with 0 < k <= m, where
 *          p is a prime and p(.) is the partition function (A000041).
 * https://oeis.org/A202650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (p, k, m) with p prime, 0 < k <= m, and
 *            n = p + partition(k) + partition(m) . The code precomputes small
 *            partition values and checks each admissible pair of partition
 *            indices against the primality of the remaining part
 * Range:     u8 n <= 449, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/*  Number of precomputed partition indices. 200 is more than
   enough for all n that appear in the tests (partition(200) > 10^9). */
#define PREK 200

/* Static storage for partition numbers. */
static seq_t *partitions = NULL;
static bool partitions_initialized = false;

/* ------------------------------------------------------------
   Pre-compute partition numbers p(0..PREK) via the classical DP
   using only non-negative operations.
   ------------------------------------------------------------ */
static void init_partitions(void)
{
    if (partitions_initialized) return;

    partitions = (seq_t *)malloc((PREK + 1) * sizeof(seq_t));
    if (!partitions) return;        /* allocation failure – unlikely */

    partitions[0] = 1;
    for (size_t i = 1; i <= PREK; ++i) partitions[i] = 0;

    for (size_t part = 1; part <= PREK; ++part) {
        for (size_t n = part; n <= PREK; ++n) {
            partitions[n] = partitions[n] + partitions[n - part];
        }
    }
    partitions_initialized = true;
}

/* ------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    init_partitions();

    seq_t N = n;               /* convert idx_t -> seq_t */
    seq_t count = 0;

    /* iterate over partition index pairs (k, m) with k <= m */
    for (size_t ki = 1; ki <= PREK; ++ki) {
        seq_t pk = partitions[ki];
        if (pk > N - 2) break;          /* smallest prime is 2 */

        for (size_t mi = ki; mi <= PREK; ++mi) {
            seq_t pm = partitions[mi];
            seq_t sum_pm = pk + pm;
            if (sum_pm > N - 2) break;   /* pm increases with mi */

            seq_t rest = N - sum_pm;     /* candidate prime p */
            if (isPrimeGeneric(rest))    /* rest is prime? */
                count = count + 1;
        }
    }
    return count;
}
