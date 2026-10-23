/*
 * A014652  Number of partitions of n in its prime divisors with at least one
 *          part of size 1.
 * https://oeis.org/A014652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of x^(n-1) in (1/(1-x))*1/Product_{d prime divisor
 *            of n} (1-x^d)
 * Range:     u8 n <= 41, u16 n <= 209, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Count partitions of n-1 into parts 1 and the distinct prime divisors of n. */
seq_t a(idx_t n)
{
    /* For n = 1, m = 0, there is a single empty partition. */
    if (n == 1)
        return (seq_t)1;

    idx_t m = n - 1;                     /* exponent we need to partition */

    /* Find distinct prime divisors of n (trial division). */
    /* n ≤ 10 000 ⇒ at most 6 distinct primes, use a small fixed buffer. */
    idx_t primes[10];
    size_t pc = 0;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pc++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pc++] = tmp;   /* remaining prime factor > sqrt(n) */

    /* Dynamic programming array: dp[i] = number of partitions of i. */
    seq_t *dp = (seq_t *)calloc((size_t)m + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;       /* allocation failure */

    dp[0] = (seq_t)1;         /* empty partition of 0 */

    /* First include part 1 */
    for (idx_t s = 1; s <= m; ++s)
        dp[s] = dp[s] + dp[s - 1];

    /* Then include each prime divisor */
    for (size_t k = 0; k < pc; ++k) {
        idx_t part = primes[k];
        for (idx_t s = part; s <= m; ++s)
            dp[s] = dp[s] + dp[s - part];
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}
