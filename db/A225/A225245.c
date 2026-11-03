/*
 * A225245  Number of partitions of n into distinct squarefree divisors of n.
 * https://oeis.org/A225245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1792, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9037 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A225245
   a(n) = number of partitions of n into distinct squarefree
   divisors of n.  Computed by generating the set of all
   squarefree divisors of n and performing a subset‑sum DP. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* NOTE: seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Fast return for n = 0 */
    if (n == 0) return 1;

    /* Factor n to obtain its distinct prime factors.  */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t nf = 0;   /* number of distinct primes */
    factor_u64((uint64_t)n, primes, exps, &nf);

    /* Generate all squarefree divisors: one for each subset of the
       prime factors (including the empty subset -> 1). */
    size_t s = ((size_t)1) << nf;
    seq_t *divs = mem_new(seq_t, s);
    if (!divs) return 0;   /* allocation failure, unlikely */

    size_t i;
    for (i = 0; i < s; ++i) {
        uint64_t prod = 1;
        size_t j;
        for (j = 0; j < nf; ++j) {
            if (i & ((size_t)1 << j))
                prod *= primes[j];
        }
        divs[i] = (seq_t)prod;
    }

    /* Subset‑sum DP: count subsets summing to each value up to n. */
    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    if (!dp) { mem_free(divs); return 0; }

    dp[0] = 1;
    for (i = 0; i < s; ++i) {
        idx_t d = (idx_t)divs[i];
        idx_t sum;
        for (sum = n; sum >= d; --sum) {
            dp[sum] += dp[sum - d];
        }
    }

    seq_t result = dp[n];
    mem_free(divs);
    mem_free(dp);
    return result;
}
