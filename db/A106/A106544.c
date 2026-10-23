/*
 * A106544  Perfect squares n^2 which are not the sum of two primes (otherwise
 *          0).
 * https://oeis.org/A106544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 if n^2 cannot be written as the sum of two primes,
 *            otherwise 0. For odd n this reduces to checking if n^2−2 is
 *            prime (since the even prime 2 must be used); for even n ≥ 2, n^2
 *            is even >2 and Goldbach’s conjecture guarantees a
 *            representation, so a(n)=0
 * Range:     u8 n <= 16, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if ((n & 1ULL) == 0ULL)
        return n == 2ULL;
    for (uint64_t i = 3ULL; i <= n / i; i += 2ULL)
    {
        if ((n % i) == 0ULL)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* we need to compute n^2; use 64‑bit arithmetic for the test */
    uint64_t nn = (uint64_t)n;
    uint64_t sq64 = nn * nn;
    seq_t sq = (seq_t)sq64;

    /* even n → n^2 is even >2 ⇒ Goldbach representation exists */
    if ((n & 1ULL) == 0ULL)
        return (seq_t)0;

    /* odd n; for n = 1 we have 1^2 - 2 = -1 ≤ 1 → not prime */
    if (sq64 <= 2ULL)
        return sq;

    /* check whether n^2-2 is prime */
    uint64_t m = sq64 - 2ULL;
    if (is_prime_uint64(m))
        return (seq_t)0;
    else
        return sq;
}
