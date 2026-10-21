/*
 * A085414  Primes that are a concatenation of a prime and its first digit.
 * https://oeis.org/A085414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth prime p such that there exists a prime q with
 *                p = concat(q, first_digit(q)).
 * Computation: enumerate prime q, construct p, and test p for primality.
 *              Uses the helper functions is_prime (for q) and
 *              is_prime_big (for p).  Only a small helper computes the
 *              most significant decimal digit of q.
 *
 *  The OEIS offset is 1: a(1) is 131.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Return the most significant decimal digit of a 64‑bit integer. */
static uint64_t first_digit_uint64(uint64_t x)
{
    while (x >= 10)
        x /= 10;
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n < 1 */

    idx_t found = 0;                    /* count of valid terms found */
    idx_t q = 2;                        /* candidate prime q */

    for (;; ++q)
    {
        if (!is_prime(q))
            continue;                   /* skip non‑prime q */

        uint64_t d = first_digit_uint64(q);   /* first (most significant) digit */

        /* p = concat(q, d) = q*10 + d */
        seq_t p = (seq_t)q * (seq_t)10 + (seq_t)d;

        if (is_prime_big(p))
        {
            ++found;
            if (found == n)
                return p;
        }
    }
}
