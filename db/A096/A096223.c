/*
 * A096223  Let p(k) be the number of partitions of k (A000041); a(n) =
 *          Sum_{1<=k<=n, gcd(k,n)=1} p(k).
 * https://oeis.org/A096223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{1<=k<=n, gcd(k,n)=1} p(k), where p(k) is the number
 *            of unrestricted partitions of k
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   gcd_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_idx.h"
#include "../utils/mem.h"

/* Helper: Euclidean algorithm for idx_t */

/* Cached array of partition numbers p[0..N] and the size of the cache. */
static seq_t *partitions = NULL;
static idx_t max_cached = 0;   /* largest k for which partitions[k] is computed */

/* Recompute all partition numbers up to n inclusively.
   The array is grown to size n+1 and filled via the classic DP recurrence. */
static void ensure_partitions(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Allocate or grow the array */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = mem_resize(seq_t, partitions, new_size);
    if (!tmp)
        abort();                      /* out‑of‑memory */
    partitions = tmp;

    /* Initialise all entries to 0 and set p(0)=1 */
    for (size_t i = 0; i <= n; ++i)
        partitions[i] = (seq_t)0;
    partitions[0] = (seq_t)1;

    /* Compute partitions by the standard unbounded‑knapsack recurrence:
           p[m] += p[m - k]  for all k <= m */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t m = k; m <= n; ++m) {
            partitions[m] += partitions[m - k];
        }
    }
    max_cached = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1, behaviour for 0 unused */
        return 0;

    ensure_partitions(n);

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd_idx(k, n) == 1)
            result += partitions[k];
    }
    return result;
}
