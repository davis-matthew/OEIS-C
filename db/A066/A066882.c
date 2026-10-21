/*
 * A066882  Number of partitions of n into prime divisors of n.
 * https://oeis.org/A066882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 629, u32 n <= 4619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066882: a(n) = number of partitions of n into prime divisors of n.
   Computed by a simple coin‑change DP over the distinct prime factors of n.
   For n = 0 return 1 (empty partition); for n = 1 return 0. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;      /* by convention */
    if (n == 1) return 0;      /* no primes divide 1 */

    uint64_t u = (uint64_t)n;

    /* factorize n: fac holds the distinct primes in ascending order */
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(u, fac, exp, &cnt);
    if (cnt == 0) return 0;     /* should not happen for n>1 */

    size_t size = (size_t)u + 1;
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp) return 0;          /* memory allocation failure */

    dp[0] = 1;

    for (int i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        for (size_t s = p; s < size; ++s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t res = dp[u];
    free(dp);
    return res;
}
