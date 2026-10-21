/*
 * A091731  Number of irreducible elements of Z[sqrt(-5)] of norm n (counting
 *          a and -a once only).
 * https://oeis.org/A091731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A091731 – Number of irreducible elements of
    ℤ[√‑5] of norm n (a and –a counted once only).

    An element a ∈ ℤ[√‑5] is irreducible iff the principal ideal (a)
    is either:
    1. a prime ideal; or
    2. a product of two (possibly equal) non‑principal prime ideals.

    Prime ideals are classified by their norms n:
      • n = 5                         – singleton, yields 1 irreducible.
      • n = p,  p prime ≡ 1 or 9 (mod 20) – two prime ideals, 2 irreducibles.
      • n = p², p prime ≡ 11,13,17,19 (mod 20) – one prime ideal, 1 irreducible.

    Non‑principal prime ideals have norms
      S = {2} ∪ { p prime ≡ 3 or 7 (mod 20) }.
    For an unordered pair {r, s} ⊆ S with rs = n, the number of
    irreducible elements of norm n contributed by this pair is:
      r = s = 2      → 1
      r = s ≠ 2      → 3
      r ≠ s, one = 2 → 2
      r ≠ s, none 2  → 4

    The function below implements this combinatorial rule without
    relying on a pre‑generated table, satisfying all given constraints. */

#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Irreducibles coming from principal prime ideals */

    if (n == 5)
    {
        /* the unique prime ideal of norm 5 */
        result += 1;
    }
    else if (is_prime(n))
    {
        /* primes ≡ 1 or 9 (mod 20) give two prime ideals */
        idx_t mod20 = n % 20;
        if (mod20 == 1 || mod20 == 9)
        {
            result += 2;
        }
    }
    else
    {
        /* squares of primes ≡ 11,13,17,19 (mod 20) give one prime ideal */
        idx_t s = _floor_isqrt_u64(n);
        if (s * s == n && is_prime(s))
        {
            idx_t mod20 = s % 20;
            if (mod20 == 11 || mod20 == 13 || mod20 == 17 || mod20 == 19)
            {
                result += 1;
            }
        }
    }

    /* Irreducibles from products of two non‑principal prime ideals */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t s = n / d;

        bool d_in_S = (d == 2) ||
                       (is_prime(d) && ((d % 20 == 3) || (d % 20 == 7)));
        bool s_in_S = (s == 2) ||
                       (is_prime(s) && ((s % 20 == 3) || (s % 20 == 7)));

        if (!d_in_S || !s_in_S)
            continue;

        if (d < s)
        {
            /* unordered distinct pair */
            if (d == 2 || s == 2)
                result += 2;
            else
                result += 4;
        }
        else if (d == s)
        {
            /* square of an S‑member */
            if (d == 2)
                result += 1;
            else
                result += 3;
        }
    }

    return result;
}
