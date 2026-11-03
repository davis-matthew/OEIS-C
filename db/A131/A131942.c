/*
 * A131942  Number of partitions of n in which each odd part has odd
 *          multiplicity.
 * https://oeis.org/A131942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 145, u64 n <= 470,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 889 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131942:
   Number of partitions of n in which each odd part has odd multiplicity.
   The generating function is
      ∏_{k≥1} (1 + q^(2k-1) – q^(4k-2)) / ((1 – q^(2k))(1 – q^(4k-2))).
   The routine computes the value directly by dynamic programming. */
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Computes a(n) for any non‑negative idx_t n.  The routine
   allocates temporary storage each call and is fully
   independent of any external memoisation. */
seq_t a(idx_t n)
{
    /* allocate arrays of size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp  = mem_new(seq_t, len);
    seq_t *tmp = mem_new(seq_t, len);
    if (!dp || !tmp) abort();

    /* initialise dp[0] = 1, all others 0 */
    mem_set(dp, 0, len * sizeof(seq_t));
    dp[0] = 1;

    /* dynamic programming over part sizes */
    for (idx_t p = 1; p <= n; ++p) {
        if ((p & 1) == 0) {                /* even part: unbounded copies */
            for (idx_t s = p; s <= n; ++s)
                dp[s] += dp[s - p];
        } else {                            /* odd part: only odd multiplicities */
            /* copy current dp into tmp */
            for (size_t i = 0; i < len; ++i)
                tmp[i] = dp[i];

            /* add contributions for all odd multiples of p */
            for (idx_t s = 0; s <= n; ++s) {
                seq_t val = dp[s];
                for (idx_t t = s + p; t <= n; t += 2 * p)
                    tmp[t] += val;
            }

            /* swap dp and tmp for next iteration */
            seq_t *o = dp;  dp = tmp;  tmp = o;
        }
    }

    /* answer is dp[n] */
    seq_t result = dp[n];
    mem_free(dp);
    mem_free(tmp);
    return result;
}
