/*
 * A066843  a(n) = Product_{k=1..n} d(k); d(k) = A000005(k) is the number of
 *          positive divisors of k.
 * https://oeis.org/A066843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ∏_{k=1..n} d(k) (d(k) = number of divisors of k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 35, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* By definition, the empty product is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array to hold divisor counts for 1..n.
       uint32_t is sufficient for d(k) ≤ n (n ≤ 10000 in test cases). */
    size_t sz = (size_t)(n + 1);
    uint32_t *tau = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!tau)
        return (seq_t)0;          /* out‑of‑memory: safe error value */

    /* Sieve to count divisors for every integer 1..n. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            ++tau[j];
    }

    /* Compute the cumulative product of the divisor counts. */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        prod = prod * (seq_t)tau[i];

    free(tau);
    return prod;
}
