/*
 * A109537  a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1; a(n)=a(n-1)+a(n-2)-a(n-4)+a(n-6)
 *          for n>=6.
 * https://oeis.org/A109537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A109537
   a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1;
   a(n)=a(n-1)+a(n-2)-a(n-4)+a(n-6)   for n≥6
*/

#include <stdlib.h>

static seq_t *dp = NULL;     /* cache of computed terms   */
static idx_t  cap = 0;       /* current allocated size    */
static idx_t  next_needed = 6;  /* first index that hasn't been computed yet */

/* Allocate or grow the dp array so it can hold at least 'needed' indices.
   The first six entries (0..5) are always set to 1. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cap) return;

    idx_t newcap = needed;
    seq_t *newdp;

#ifdef __cplusplus
    /* C++: use new[]/delete[] so that constructed types are handled correctly. */
    newdp = new seq_t[newcap];
    if (dp) {
        for (idx_t i = 0; i < cap; ++i) newdp[i] = dp[i];
        delete[] dp;
    }
    dp = newdp;
    cap = newcap;
#else
    /* C: realloc preserves existing contents. */
    newdp = (dp == NULL)
        ? (seq_t *)malloc(newcap * sizeof(seq_t))
        : (seq_t *)realloc(dp, newcap * sizeof(seq_t));
    if (!newdp) abort();            /* allocation failed */
    dp = newdp;
    cap = newcap;
#endif

    /* initialise the mandatory first terms */
    for (idx_t i = 0; i < 6 && i < cap; ++i) dp[i] = (seq_t)1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)1;  /* trivial case */

    ensure_capacity(n + 1);

    for (idx_t i = next_needed; i <= n; ++i)
        dp[i] = dp[i - 1] + dp[i - 2] - dp[i - 4] + dp[i - 6];

    next_needed = n + 1;
    return dp[n];
}
