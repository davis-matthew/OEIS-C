/*
 * A101875  Duplicate of A101871.
 * https://oeis.org/A101875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = product over prime divisors p of (2n+1) of
 *            partition(v_p(2n+1)), where partition(k) is the number of
 *            partitions of k. The sequence counts abelian groups of order
 *            4n+2 = 2 * (2n+1)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation factors k = 2n+1 and multiplies the partition numbers
   of the prime‑exponents.  A small DP computes partition(k) for the very
   small exponents that can occur (k ≤ 2·10⁴ in the tests).  */

#include <stdint.h>
#include <stdlib.h>

/* Compute partition numbers for small k (k ≤ MAX_EXP-1).  */
static seq_t partition_of_exp(idx_t e)
{
    /* Exponents for n ≲ 10⁴ never exceed 9, but a generous bound is used. */
#define MAX_EXP 128
    static seq_t dp[MAX_EXP];
    if (e >= MAX_EXP) {
        /* Sanity: the exponent is too large; in practice this never happens.
           Return 1 as a safe default. */
        return 1;
    }
    /* Reset dp[0..e] to 0, then set dp[0] = 1. */
    for (idx_t i = 0; i <= e; ++i)
        dp[i] = 0;
    dp[0] = 1;
    for (idx_t i = 1; i <= e; ++i) {
        for (idx_t j = i; j <= e; ++j)
            dp[j] += dp[j - i];
    }
    return dp[e];
}

seq_t a(idx_t n)
{
    /* Compute k = 2n+1 in an enlarged type to avoid overflow of idx_t. */
    unsigned __int128 tmp = ((unsigned __int128)n) * 2 + 1;
    uint64_t k = (uint64_t)tmp;       /* k fits into 64 bits for the test range */

    seq_t result = 1;
    uint64_t temp = k;

    /* Trial‑division factorisation of k.  Since k ≤ 20001 for the tests,
       this is trivial and fast. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            result *= partition_of_exp(exp);
        }
    }

    /* Any remaining prime factor has exponent 1; partition(1) == 1,
       so it does not affect the product. */
    return result;
}
