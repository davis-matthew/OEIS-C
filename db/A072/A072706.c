/*
 * A072706  Number of unimodal partitions/compositions of n into distinct
 *          terms.
 * https://oeis.org/A072706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * ( 1 + Π_{k>0} (1 + 2*x^k) ) – coefficient of x^n in
 *            the right side
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 139, u64 n <= 459,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count unimodal compositions of n into distinct parts. */
#include <stdlib.h>

/* The type seq_t may be an unsigned integer of any size, including an
   arbitrary‑precision integer class.  For intermediate computations we
   use an unsigned __int128 so that all 64‑bit builds avoid overflow. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of 128‑bit integers for the DP. */
    size_t len = (size_t)n + 1;
    unsigned __int128 *dp = (unsigned __int128*)calloc(len, sizeof(unsigned __int128));
    if (!dp)
        return (seq_t)0;           /* allocation failure – unlikely in tests */

    dp[0] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update from high to low to keep old values untouched when
           needed for the same k. */
        for (idx_t s = n; s >= k; --s) {
            dp[s] += (dp[s - k] << 1);      /* 2 * dp[s-k] */
        }
    }

    seq_t result = (seq_t)(dp[n] >> 1);      /* divide by 2 */
    free(dp);
    return result;
}
