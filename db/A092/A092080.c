/*
 * A092080  Triangle read by rows in which row n lists the partition numbers
 *          of the first n positive integers.
 * https://oeis.org/A092080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = p( n - t ), where t is the largest triangular number < n
 *            (t = k*(k+1)/2 with k = floor((sqrt(8*n-7)-1)/2) ). The function
 *            uses DP coin‑change to compute partition numbers up to the
 *            needed size
 * Range:     u8 n <= 8, u16 n <= 989, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *partition_cache = NULL;   /* array for p(0)…p(N)          */
static size_t  partition_cap   = 0;     /* current capacity (N+1)       */

/* Recompute partition table up to N (inclusive) using the classic DP
   coin‑change algorithm:  p[0] = 1; for coin = 1…N, for sum = coin…N
   p[sum] += p[sum-coin];  All operations are on seq_t.
   Allocation is performed on each call to keep code simple. */
static void recompute_partitions(idx_t N)
{
    seq_t *new_cache = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!new_cache) exit(1);                 /* allocation failure */
    new_cache[0] = (seq_t)1;                 /* base case: p(0) = 1 */

    for (idx_t coin = 1; coin <= N; ++coin) {
        for (idx_t sum = coin; sum <= N; ++sum) {
            new_cache[sum] += new_cache[sum - coin];
        }
    }

    /* Free previous cache and store the new one */
    free(partition_cache);
    partition_cache = new_cache;
    partition_cap   = N + 1;
}

/* Ensure that partition_cache contains p(0)…p(N).  Recompute only
   if N >= current capacity. */
static void ensure_partitions(idx_t N)
{
    if (N + 1 > partition_cap)
        recompute_partitions(N);
}

/* Main function: a(n) as defined in A092080 */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the official sequence, but handle gracefully */
        return (seq_t)0;

    /* Compute k = largest integer with k*(k+1)/2 < n.
       Use integer square root to avoid floating point. */
    seq_t tmp = (seq_t)8 * (seq_t)n - 7;            /* 8*n - 7  */
    seq_t sq  = floor_sqrt(tmp);                   /* floor sqrt */
    idx_t  k  = ((idx_t)(sq - 1)) / 2;              /* k such that T_k < n ≤ T_{k+1} */

    seq_t tk  = (seq_t)k * (k + 1) / 2;             /* T_k  */
    idx_t  m  = (idx_t)(n - (idx_t)tk);            /* n - T_k */

    ensure_partitions(m);                          /* p(0)…p(m) */
    return partition_cache[m];                     /* a(n) = p(m) */
}
