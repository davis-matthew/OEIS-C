/*
 * A195851  Column 7 of array A195825. Also column 1 of triangle A195841. Also
 *          1 together with the row sums of triangle A195841.
 * https://oeis.org/A195851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts ≡ 0, 1 or 8 modulo 9
 * Range:     u8 n <= 52, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

/* The dynamic programming table of partition counts.  */
static seq_t *dp = NULL;     /* array of a values, length = dp_size */
static idx_t dp_size = 0;    /* number of elements stored in dp */

/* Recompute a[0..N] (inclusive).  The table is grown at once to avoid
 * costly reallocations.  No old memory is freed to keep the code
 * compatible with the arbitrary-precision seq_t allocator.
 */
static void recompute(idx_t N)
{
    /* Allocate space for N+1 values.  seq_alloc returns a seq_t* array. */
    seq_t *newdp = seq_alloc(N + 1);

    /* Initialize all entries to zero and set a(0)=1. */
    for (idx_t i = 0; i <= N; ++i)
        newdp[i] = (seq_t)0;
    newdp[0] = (seq_t)1;

    /* Handle parts that are multiples of 9: 9,18,27,... */
    for (idx_t k = 1; 9 * k <= N; ++k) {
        idx_t p = 9 * k;
        for (idx_t i = p; i <= N; ++i)
            newdp[i] += newdp[i - p];
    }

    /* Handle parts that are 1 modulo 9: 1,10,19,...  */
    for (idx_t k = 1; 9 * k - 1 <= N; ++k) {
        idx_t p = 9 * k - 1;
        for (idx_t i = p; i <= N; ++i)
            newdp[i] += newdp[i - p];
    }

    /* Handle parts that are 8 modulo 9: 8,17,26,...  */
    for (idx_t k = 1; 9 * k - 8 <= N; ++k) {
        idx_t p = 9 * k - 8;
        for (idx_t i = p; i <= N; ++i)
            newdp[i] += newdp[i - p];
    }

    /* Replace the old table.  The old data is intentionally left
     * unfreed to avoid mismatches between seq_alloc and seq_free.  */
    dp = newdp;
    dp_size = N + 1;
}

seq_t a(idx_t n)
{
    if (!dp || n >= dp_size)
        recompute(n);
    return dp[n];
}
