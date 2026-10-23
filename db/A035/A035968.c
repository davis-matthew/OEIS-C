/*
 * A035968  Number of partitions of n into parts not of the form 17k, 17k+7 or
 *          17k-7. Also number of partitions with at most 6 parts of size 1
 *          and differences between parts at distance 7 are greater than 1.
 * https://oeis.org/A035968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   array_new
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A035968
   Formula: a(n) = number of partitions of n into parts not congruent to
               0, 7 or 10 modulo 17   (i.e. not of the form 17k, 17k+7, 17k-7)
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/array_new.h"

static void array_free(seq_t *p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

/* ---------- memoised DP -------------------------------------- */
static seq_t *dp = NULL;           /* dp[0..max_n] */
static idx_t  max_n = 0;

/* recompute dp[0..n] from scratch */
static void recompute_up_to(idx_t n)
{
    seq_t *new_dp = array_new(n + 1);
    if (!new_dp) abort();

    /* initialise all entries to 0 (new[] is not guaranteed zero) */
    for (idx_t i = 0; i <= n; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;                 /* empty partition */

    for (idx_t part = 1; part <= n; ++part)
    {
        idx_t mod = part % 17;
        if (mod == 0 || mod == 7 || mod == 10) continue;  /* forbidden parts */

        for (idx_t i = part; i <= n; ++i)
            new_dp[i] += new_dp[i - part];
    }

    if (dp) array_free(dp);
    dp = new_dp;
    max_n = n;
}

/* ---------- public routine ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined below offset 1 */

    if (n > max_n) recompute_up_to(n);
    return dp[n];
}
