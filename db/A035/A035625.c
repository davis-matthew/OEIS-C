/*
 * A035625  Number of partitions of n into parts 4k+1 and 4k+3 with at least
 *          one part of each type.
 * https://oeis.org/A035625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 4k+1 and 4k+3 with
 *            at least one part of each type. This equals p_odd(n) - p_1(n) -
 *            p_3(n), where p_odd counts partitions into odd parts, p_1 into
 *            parts ≡1 (mod 4), and p_3 into parts ≡3 (mod 4)
 * Range:     u8 n <= 30, u16 n <= 78, u32 n <= 237, u64 n <= 791,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

/* Cached tables.  They are recomputed from scratch whenever a larger limit
   is required.  Static linkage ensures memoisation across calls. */
static seq_t *dp_odd = NULL;
static seq_t *dp_1   = NULL;
static seq_t *dp_3   = NULL;
static idx_t  cache_limit = 0;

/* Allocate fresh tables of size (n+1) and initialise them to zero.  For
   C++ the new operator value‑initialises the array, for C we use malloc
   followed by memset. */
static void alloc_tables(idx_t n)
{
    /* Free old tables if any */
    if (dp_odd) {
        #ifdef __cplusplus
            delete[] dp_odd;
            delete[] dp_1;
            delete[] dp_3;
        #else
            free(dp_odd);
            free(dp_1);
            free(dp_3);
        #endif
    }

    /* Allocate new tables */
    #ifdef __cplusplus
        dp_odd = new seq_t[n + 1]();
        dp_1   = new seq_t[n + 1]();
        dp_3   = new seq_t[n + 1]();
    #else
        dp_odd = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        dp_1   = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        dp_3   = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        memset(dp_odd, 0, sizeof(seq_t) * (n + 1));
        memset(dp_1,   0, sizeof(seq_t) * (n + 1));
        memset(dp_3,   0, sizeof(seq_t) * (n + 1));
    #endif

    /* Table initialisation: dp[0] = 1 (empty partition) */
    dp_odd[0] = 1;
    dp_1[0]   = 1;
    dp_3[0]   = 1;
    cache_limit = n;
}

/* Recompute the tables up to the required limit.  The cost is O(n²)
   but is acceptable for n up to about 10 000, as required. */
static void recompute_up_to(idx_t n)
{
    if (n <= cache_limit)
        return;
    alloc_tables(n);

    /* Partitions into all odd parts */
    for (idx_t coin = 1; coin <= n; coin += 2) {
        for (idx_t m = coin; m <= n; ++m) {
            dp_odd[m] += dp_odd[m - coin];
        }
    }

    /* Partitions into parts ≡1 (mod 4) */
    for (idx_t coin = 1; coin <= n; coin += 4) {
        for (idx_t m = coin; m <= n; ++m) {
            dp_1[m] += dp_1[m - coin];
        }
    }

    /* Partitions into parts ≡3 (mod 4) */
    for (idx_t coin = 3; coin <= n; coin += 4) {
        for (idx_t m = coin; m <= n; ++m) {
            dp_3[m] += dp_3[m - coin];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1 */
        return 0;

    recompute_up_to(n);
    /* Subtraction is safe because each of the three counts is
       non‑negative and dp_odd[n] >= dp_1[n] + dp_3[n] for n>0. */
    return dp_odd[n] - dp_1[n] - dp_3[n];
}
