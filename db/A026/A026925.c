/*
 * A026925  Number of partitions of n into an odd number of parts, the
 *          greatest being 5; also, a(n+9) = number of partitions of n+4 into
 *          an even number of parts, each <=5.
 * https://oeis.org/A026925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an odd number of parts,
 *            the greatest part being 5. This equals the number of partitions
 *            of n into parts ≤ 5 with an odd number of parts minus those into
 *            parts ≤ 4 with an odd number of parts. The values are computed
 *            by dynamic programming in O(n·5)
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static DP tables and cache size */
static seq_t *odd5 = NULL, *even5 = NULL;
static seq_t *odd4 = NULL, *even4 = NULL;
static idx_t  cache_max = 0;

/* Recompute DP tables up to N (inclusive) */
static void _ensure_cache(idx_t N)
{
    if (N <= cache_max) return;               /* already up‑to‑date */

    /* Allocate zeroed tables of size N+1 */
    size_t sz = (size_t)(N + 1);

    free(odd5);  free(even5);  free(odd4);  free(even4);
    odd5  = (seq_t*)calloc(sz, sizeof(seq_t));
    even5 = (seq_t*)calloc(sz, sizeof(seq_t));
    odd4  = (seq_t*)calloc(sz, sizeof(seq_t));
    even4 = (seq_t*)calloc(sz, sizeof(seq_t));

    if (!odd5 || !even5 || !odd4 || !even4) { /* allocation failed */
        /* In a real system we would handle this error. For the
           purposes of the exercise we assume enough memory. */
        return;
    }

    /* Base cases: empty partition */
    even5[0] = 1;
    even4[0] = 1;

    /* Partitions with parts ≤ 5 */
    for (idx_t v = 1; v <= 5; ++v) {
        for (idx_t s = v; s <= N; ++s) {
            odd5[s]  += even5[s - v];
            even5[s] += odd5[s - v];
        }
    }

    /* Partitions with parts ≤ 4 */
    for (idx_t v = 1; v <= 4; ++v) {
        for (idx_t s = v; s <= N; ++s) {
            odd4[s]  += even4[s - v];
            even4[s] += odd4[s - v];
        }
    }

    cache_max = N;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; values below 1 are unused */
    _ensure_cache(n);
    return odd5[n] - odd4[n];
}
