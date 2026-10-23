/*
 * A189024  Number of primes in the range (n - sqrt(n), n].
 * https://oeis.org/A189024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{prime p : n - sqrt(n) < p ≤ n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Very small prime test, adequate for the range needed (n ≤ 10 000). */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1U) == 0) return false;          /* even numbers > 2 are not prime */
    for (idx_t i = 3; i * i <= x; i += 2)     /* trial division by odd divisors */
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                      /* no primes below 2 */

    /* Compute ceil(sqrt(n)) by incremental search.  */
    idx_t ceil_sqrt = 0;
    while (ceil_sqrt * ceil_sqrt < n) ++ceil_sqrt;

    /* First integer strictly greater than n - sqrt(n). */
    idx_t start = n - ceil_sqrt + 1;
    if (start < 2) start = 2;                 /* primes start at 2 */

    seq_t count = 0;
    for (idx_t p = start; p <= n; ++p)
        if (is_prime_idx(p)) ++count;

    return count;
}
