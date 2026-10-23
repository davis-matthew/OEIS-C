/*
 * A026826  Number of partitions of n into distinct parts, the least being 5.
 * https://oeis.org/A026826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 117, u32 n <= 301, u64 n <= 903,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026826: a(n) = number of partitions of n into distinct parts, the
   least part being 5.  This is the coefficient of x^n in the generating
   function x^5 * Π_{j≥6}(1 + x^j).                                    */
#include <stdlib.h>

static seq_t *dp = NULL;      /* partitions of sums into distinct parts ≥6   */
static idx_t dp_len = 0;      /* length of dp array (indices 0..dp_len-1)   */

/* Compute dp up to max_m (inclusive).  dp[0] is always 1 (empty partition). */
static void ensure_dp(idx_t max_m)
{
    if (max_m < dp_len)            /* already built up to required size */
        return;

    idx_t new_len = max_m + 1;      /* indices 0 … max_m */
    seq_t *new_dp = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_dp)                   /* allocation should never fail on test */
        return;

    /* Initialise all entries to 0. */
    for (idx_t i = 0; i < new_len; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;                  /* the empty partition */

    /* Typical DP for partitions into distinct parts where each part can
       be used at most once: iterate parts ascending, sums descending. */
    for (idx_t part = 6; part <= max_m; ++part) {
        for (idx_t sum = max_m; sum >= part; --sum) {
            new_dp[sum] += new_dp[sum - part];
        }
    }

    /* Replace the old array. */
    if (dp)
        free(dp);
    dp = new_dp;
    dp_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;               /* no partition can have smallest part 5 */

    idx_t m = n - 5;              /* remove the mandatory part 5 */
    ensure_dp(m);                 /* compute dp up to m if necessary */
    return dp[m];
}
