/*
 * A158181  Number of nondecreasing integer sequences of length 47 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{l=1}^{min(n,47)} \;\sum_{k=1}^{min(n,47-l)} P(n,l)
 *            * P(n,k) where P(n,l) is the number of partitions of n into
 *            exactly l parts. This counts nondecreasing integer sequences of
 *            length 47 with sum zero and sum of absolute values 2n. For n <=
 *            23 the inner sum contains all partitions of n...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 132, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define PART_MAX 48   /* maximum parts to consider (0..47) */

/* static storage for partition table */
static seq_t *dp = NULL;          /* dp[n * PART_MAX + j] = P(n,j) */
static idx_t  dp_alloc_n = 0;     /* largest n for which dp is filled */

/* ensure that dp contains values up to given nmax */
static void ensure_dp(idx_t nmax)
{
    if (nmax <= dp_alloc_n)
        return;

    size_t new_size = (size_t)(nmax + 1) * PART_MAX;
    seq_t *newdp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!newdp)
        exit(1);                  /* out of memory */

    /* initialize new rows to zero */
    for (idx_t i = dp_alloc_n + 1; i <= nmax; ++i)
        for (idx_t j = 0; j < PART_MAX; ++j)
            newdp[i * PART_MAX + j] = 0;

    /* base case: P(0,0) = 1 */
    newdp[0 * PART_MAX + 0] = 1;

    /* compute new rows using recurrence:
       P(n,j) = P(n-1,j-1) + P(n-j,j)  for 1 <= j <= min(47,n)                 */
    for (idx_t n = dp_alloc_n + 1; n <= nmax; ++n) {
        for (idx_t j = 1; j <= 47 && j <= n; ++j) {
            seq_t t1 = newdp[(n - 1) * PART_MAX + (j - 1)];
            seq_t t2 = (n >= j) ? newdp[(n - j) * PART_MAX + j] : 0;
            newdp[n * PART_MAX + j] = t1 + t2;
        }
    }

    dp = newdp;
    dp_alloc_n = nmax;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_dp(n);

    seq_t prefix[PART_MAX];
    prefix[0] = 0;
    for (idx_t j = 1; j < PART_MAX; ++j)
        prefix[j] = prefix[j - 1] + dp[n * PART_MAX + j];

    seq_t result = 0;
    idx_t maxL = (n < 47) ? n : 47;

    for (idx_t l = 1; l <= maxL; ++l) {
        seq_t cntL = dp[n * PART_MAX + l];
        if (!cntL)
            continue;
        idx_t limitK = 47 - l;
        if (limitK == 0)
            continue;              /* no k can satisfy l + k <= 47 */
        seq_t sumK = prefix[limitK];
        result += cntL * sumK;
    }

    return result;
}
