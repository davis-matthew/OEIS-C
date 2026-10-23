/*
 * A171487  Product of odd prime anti-factors < n, with multiplicity.
 * https://oeis.org/A171487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (isPrime(2n-1) ? 1 : 2n-1) * (isPrime(2n+1) ? 1 : 2n+1)
 * Range:     u8 n <= 12, u16 n <= 129, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the product of odd prime anti-factors < n (with multiplicity) */
/* Works for any n ≥ 1; uses the provided is_prime routine. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t m1 = (idx_t)2 * n - 1;
    idx_t m2 = (idx_t)2 * n + 1;
    seq_t result = 1;

    if (!is_prime(m1))
        result *= (seq_t)m1;
    if (!is_prime(m2))
        result *= (seq_t)m2;

    return result;
}
