/*
 * A109701  Number of partitions of n into parts each equal to 1 mod 6.
 * https://oeis.org/A109701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 216, u32 n <= 672, u64 n <= 2284,
 *            u128 n <= 8257, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition numbers into parts congruent to 1 (mod 6)
   Recurrence:   a(0)=1
                 a(n)=∑_{k=0}^{n} a(n-(1+6k)) ,  with a(m)=0 for m<0
   (Equivalent to the generating function 1/∏_{j≥0}(1-x^{1+6j})).   */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;   /* DP array holding a(0) … a(max) */
static idx_t  dp_max = 0;  /* largest n for which dp has been computed */

/* Allocate a fresh DP array of length N+1 and compute all values up to N */
static void recompute_to(idx_t N)
{
    /* Allocate a new array large enough for indices 0 … N */
    seq_t *new_dp = alloc_seq_array((size_t)(N + 1));
    if (!new_dp) exit(1);   /* abort on allocation failure */

    /* Initialise all entries to zero */
    for (idx_t i = 0; i <= N; ++i)
        new_dp[i] = (seq_t)0;

    /* Base case */
    new_dp[0] = (seq_t)1;

    /* Add contributions of all admissible part sizes (1 mod 6) */
    for (idx_t part = 1; part <= N; part += 6) {
        for (idx_t i = part; i <= N; ++i) {
            new_dp[i] = new_dp[i] + new_dp[i - part];
        }
    }

    /* Replace the old DP array with the new one */
    if (dp)
        free_seq_array(dp);
    dp     = new_dp;
    dp_max = N;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    /* Initialise on first use */
    if (!dp)
        recompute_to(0);

    /* Grow the DP table if needed */
    if (n > dp_max)
        recompute_to(n);

    return dp[n];
}
