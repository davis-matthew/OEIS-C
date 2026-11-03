/*
 * A107078  Whether n has non-unitary prime divisors.
 * https://oeis.org/A107078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n has a non‑unitary prime divisor (i.e. n is not
 *            square‑free), 0 otherwise. Equivalently a(n) = 1 - μ(n)^2
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 117 terms through n = 4255 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff n has any squared prime divisor. */
static bool has_square_prime(idx_t n)
{
    /* 1, 2, 3 are trivially square‑free. */
    if (n < 4) return false;

    /* Try all primes p up to sqrt(n).  Using division instead of
       128–bit multiplication keeps the code portable.  The loop
       condition p <= n/p avoids overflow. */
    for (idx_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            /* If p divides n twice then n is divisible by p². */
            idx_t q = n / p;
            if (q % p == 0)
                return true;
        }
    }
    return false;
}

/* External sequence function required by the test harness. */
seq_t a(idx_t n)
{
    /* n is the index, which is also the argument of the mathematical
       definition.  We return 1 when n is not square‑free, otherwise 0. */
    return (seq_t)(has_square_prime(n) ? 1 : 0);
}
