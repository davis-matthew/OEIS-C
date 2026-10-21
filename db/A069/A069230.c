/*
 * A069230  Number of primes p such that n < p < n + tau(n)^2 where tau(n) =
 *          A000005(n).
 * https://oeis.org/A069230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{p prime : n < p < n + τ(n)^2}|, τ(n)=A000005(n) (offset
 *            1)
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return the number of divisors of n (τ(n)).  The result fits in idx_t
   for the ranges we need (n ≤ 10000).  It is returned as idx_t so that
   we can use it directly for bounds without converting from an
   arbitrary‑precision seq_t. */
static idx_t divisor_count_u64(idx_t n)
{
    idx_t count = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (m > 1)      /* remaining prime factor */
        count *= 2;
    return count;
}

/* Deterministic primality test for unsigned 64‑bit integers.
   For the range of n we need (together with the search interval)
   this simple test is fast enough. */
static bool is_prime_u64(idx_t n)
{
    if (n < 2)          return false;
    if (n == 2 || n == 3)
        return true;
    if ((n & 1ULL) == 0)    /* even numbers > 2 are composite */
        return false;

    for (idx_t p = 3; p * p <= n; p += 2)
        if (n % p == 0)
            return false;
    return true;
}

/* Main sequence function.  The function works for any seq_t that
   supports conversion from a small integer and for idx_t = uint64_t. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; a(0) undefined */

    /* Compute τ(n).  We do this with idx_t arithmetic so that we can
       safely use the result for bounds without worrying about
       arbitrary‑precision conversions. */
    idx_t tau = divisor_count_u64(n);

    /* Span to search for primes: τ(n)^2. */
    idx_t span = tau * tau;               /* fits in idx_t for all n < 10000 */

    idx_t start = n + 1;                  /* primes must be strictly > n */
    idx_t end   = n + span;               /* exclusive upper bound */

    idx_t primes = 0;
    for (idx_t x = start; x < end; ++x)
        if (is_prime_u64(x))
            ++primes;

    return (seq_t)primes;
}
