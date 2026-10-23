/*
 * A035451  Number of partitions of n into parts congruent to 1 mod 4.
 * https://oeis.org/A035451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 147, u32 n <= 455, u64 n <= 1538,
 *            u128 n <= 5540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, a(n) = Σ_{k≥0, 4k+1≤n} a(n-(4k+1)) */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;          /* dp[i] = a(i) */
    static size_t allocated = 0;      /* length of the allocated array */
    static idx_t curr_max = 0;        /* highest n for which dp[0..curr_max] is up to date */

    /* Initial allocation if there is none */
    if (dp == NULL) {
        allocated = (size_t)(n + 1);
        dp = (seq_t *)calloc(allocated, sizeof(seq_t));
        if (!dp)
            return (seq_t)0;          /* allocation failed */
        dp[0] = 1;                    /* a(0) = 1 */
        curr_max = 0;
    }

    /* Grow array if needed */
    if ((size_t)(n + 1) > allocated) {
        size_t new_alloc = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_alloc * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed */
        /* Zero the newly allocated tail */
        memset(tmp + allocated, 0,
               (new_alloc - allocated) * sizeof(seq_t));
        dp = tmp;
        allocated = new_alloc;
    }

    /* Re‑compute dp up to n if we are asked for a new larger value */
    if (n > curr_max) {
        /* Initialise the array for the new range */
        memset(dp, 0, (n + 1) * sizeof(seq_t));
        dp[0] = 1;                    /* base case */

        /* Standard coin–change DP: iterate over each allowed part 1 (mod 4) */
        for (idx_t coin = 1; coin <= n; coin += 4) {
            for (idx_t j = coin; j <= n; ++j) {
                dp[j] += dp[j - coin];
            }
        }
        curr_max = n;
    }

    return dp[n];
}
