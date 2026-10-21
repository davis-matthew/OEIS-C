/*
 * A069183  Expansion of 1/((1-x)*(1-x^2)^2*(1-x^3)*(1-x^6)).
 * https://oeis.org/A069183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of partitions of n into parts 1, 2, 2, 3, 6
 *            (i.e. the coefficient of x^n in 1/((1‑x)*(1‑x²)²*(1‑x³)*(1‑x⁶))
 * Range:     u8 n <= 19, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The array dp stores all already computed terms up to dp_cap‑1.
   It is expanded only when a larger n is requested.  No private
   global variables other than dp and dp_cap are used. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;          /* cache of computed terms */
static idx_t dp_cap = 0;          /* first index that is not yet known */

/* Compute all terms up to new_cap–1 (inclusive).  The computation
   uses the standard coin‑change dynamic programming with the coin
   multiset {1,2,2,3,6}. */
static void recompute_all(idx_t new_cap)
{
    /* Allocate a new buffer.  alloc_seq_array returns a correctly
       constructed array of seq_t for both C and C++. */
    seq_t *new_dp = alloc_seq_array((size_t)new_cap);
    if (!new_dp) return;                /* out of memory – leave dp untouched */

    /* Initialise to zero; dp[0]=1 will be set afterwards. */
    for (idx_t i = 0; i < new_cap; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;

    const int coins[5] = {1, 2, 2, 3, 6};

    for (int ci = 0; ci < 5; ++ci) {
        int c = coins[ci];
        for (idx_t i = (idx_t)c; i < new_cap; ++i)
            new_dp[i] += new_dp[i - c];
    }

    /* Replace the old cache.  free_seq_array correctly destroys the
       array elements for arbitrary‑precision seq_t. */
    if (dp) free_seq_array(dp);
    dp     = new_dp;
    dp_cap = new_cap;
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (dp_cap <= n)
        recompute_all(n + 1);   /* grow cache to include n */
    return dp[n];
}
