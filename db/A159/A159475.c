/*
 * A159475  a(1) = 1; for n >= 1, a(n) is the smallest number m > n such that
 *          gcd(n,m) > 1.
 * https://oeis.org/A159475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + smallest prime factor of n (a(1) = 1)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the smallest prime factor of n (for n ≥ 2).
   If n is prime, return n itself.  For n ≤ 1 the value 0 is returned
   but a(1) is handled specially outside. */
static uint64_t spf_uint64(uint64_t n)
{
    if (n <= 1)
        return 0;
    if ((n & 1) == 0)      /* even */
        return 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0)
            return i;
    return n;               /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t p = spf_uint64((uint64_t)n);
    return (seq_t)n + (seq_t)p;
}
