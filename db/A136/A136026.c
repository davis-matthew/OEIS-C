/*
 * A136026  Smallest prime of the form (2n+1)p + 2n with p prime.
 * https://oeis.org/A136026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 703, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the smallest prime of the form (2n+1)*p + 2n, where p is prime. */

#include <stdint.h>
#include <stdbool.h>

/* Primitive prime test for 64‑bit values (trial division). */
static bool is_prime_u64(uint64_t n) {
    if (n < 2)                  return false;
    if (n == 2 || n == 3)       return true;
    if ((n & 1ULL) == 0)         return false;   /* even */
    if (n % 3ULL == 0)           return false;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0)         return false;
        if (n % (i + 2) == 0)   return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. For n = 0 return 0
       (undefined but harmless for callers). */
    if (n == 0) return 0;

    /* Compute the coefficients for the formula (2n+1)*p + 2n. */
    uint64_t f   = 2 * n + 1;   /* factor (2n+1)   */
    uint64_t off = 2 * n;       /* offset (2n)     */

    /* Enumerate primes p and test the resulting q. */
    for (uint64_t p = 2; ; ++p) {
        if (!is_prime_u64(p))
            continue;

        uint64_t q = f * p + off;
        if (is_prime_u64(q))
            return (seq_t)q;   /* q fits into seq_t for all required arguments */
    }

    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
