/*
 * A035960  Number of partitions of n into parts not of the form 15k, 15k+6 or
 *          15k-6. Also number of partitions with at most 5 parts of size 1
 *          and differences between parts at distance 6 are greater than 1.
 * https://oeis.org/A035960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            6 or 9 modulo 15. Computation: standard unbounded‑knapsack DP
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1, harmless */

    size_t N = (size_t)n + 1; /* array size */
    seq_t *dp = alloc_seq_array(N);   /* zero‑initialised array of seq_t */
    if (!dp)
        abort();

    dp[0] = 1;               /* empty partition */

    for (idx_t s = 1; s <= n; ++s) {
        int r = (int)(s % 15);
        if (r == 0 || r == 6 || r == 9)   /* forbidden residues */
            continue;
        for (idx_t i = s; i <= n; ++i)
            dp[i] += dp[i - s];
    }

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
