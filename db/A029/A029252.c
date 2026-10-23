/*
 * A029252  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A029252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 4, 6, and 8
 * Range:     u8 n <= 83, u16 n <= 597, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed values */
static seq_t *dp = NULL;        /* dp[i] holds a(i) for 0 <= i < dp_size */
static idx_t  dp_size = 0;      /* number of elements stored (last index + 1) */

/* Allowed part sizes as idx_t so we can index the array safely */
static const idx_t parts[] = {3, 4, 6, 8};
static const size_t parts_cnt = sizeof(parts) / sizeof(parts[0]);

/* Ensure that dp[n] is computed.  If n is larger than the current
   cache, recompute the entire table up to n.  If n is already cached,
   do nothing. */
static void ensure_up_to(idx_t n)
{
    if (dp_size > n)
        return;                     /* already computed, nothing to do */

    /* Allocate or enlarge the cache */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(dp, (size_t)new_size * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure */
    dp = tmp;

    /* Initialise all entries up to n */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                       /* empty partition */

    /* Classic coin‑change DP: for each part add its contribution to
       all larger indices.  This generates the number of unordered
       partitions. */
    for (size_t pi = 0; pi < parts_cnt; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    dp_size = new_size;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return dp[n];
}
