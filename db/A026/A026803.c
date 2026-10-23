/*
 * A026803  Number of partitions of n in which the least part is 10.
 * https://oeis.org/A026803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n in which the least part is 10.
 *            The algorithm counts partitions of n‑10 into parts ≥10 and adds
 *            one part of size 10. A one‑dimensional dynamic programming table
 *            is used
 * Range:     u8 n <= 70, u16 n <= 129, u32 n <= 285, u64 n <= 737,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;   /* dp[j] = #partitions of j with parts ≥10    */
static idx_t max_m = 0;    /* largest j for which dp is computed       */

/* Allocate and initialise a table up to new_m.  The table is built from
   scratch; earlier values are not reused.  This guarantees that overflow
   in large indices does not corrupt smaller ones. */
static void recompute(idx_t new_m)
{
    size_t sz = (size_t)(new_m + 1) * sizeof(seq_t);
    seq_t *newdp = (seq_t *)malloc(sz);
    if (!newdp) return;                     /* allocation failed – abort */

    /* start with zero entries, dp[0] = 1 (empty partition) */
    for (idx_t i = 0; i <= new_m; ++i)
        newdp[i] = 0;
    newdp[0] = 1;

    /* classic coin‑change DP using coins 10..new_m */
    for (idx_t part = 10; part <= new_m; ++part) {
        for (idx_t j = part; j <= new_m; ++j) {
            newdp[j] = newdp[j] + newdp[j - part];
        }
    }

    free(dp);
    dp = newdp;
    max_m = new_m;
}

seq_t a(idx_t n)
{
    /* For n < 10 there is no partition with least part 10. */
    if (n < 10) return 0;

    /* Ensure the table is initialised for 0. */
    if (dp == NULL) recompute(0);

    idx_t m = n - 10;              /* we need partitions of m with parts ≥10 */

    /* Extend the table if necessary. */
    if (m > max_m) recompute(m);

    return dp[m];
}
