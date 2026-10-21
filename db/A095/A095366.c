/*
 * A095366  Least k > 1 such that k divides 1^n + 2^n +...+ (k-1)^n.
 * https://oeis.org/A095366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least prime p > 1 such that (p-1) does not divide n
 * Range:     u8 n <= 9202, u16 n <= 9202, u32 n <= 9202, u64 n <= 9202,
 *            u128 n <= 9202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation checks each odd integer ≥ 3 for primality and tests the divisor condition. */

#include <stdbool.h>

/* Trial division primality test suitable for small integers (≤ 2·10^6).   */
static bool is_prime_small(idx_t x)
{
    if (x < 2) return false;
    if ((x & 1u) == 0) return x == 2u;          /* even numbers */
    for (idx_t i = 3; i <= x / i; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    for (idx_t p = 3; ; p += 2)            /* start with the first odd prime candidate */
    {
        if (!is_prime_small(p)) continue; /* skip composites */
        if (n % (p - 1) != 0)            /* (p-1) ∤ n  →  p divides the sum  */
            return (seq_t)p;
    }
}
