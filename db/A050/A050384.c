/*
 * A050384  Nonprimes such that n and phi(n) are relatively prime.
 * https://oeis.org/A050384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth non‑prime integer such that gcd(n, φ(n)) = 1, with
 *            a(1) = 1. This implementation searches integers in ascending
 *            order, skipping primes and testing the coprimality condition via
 *            the provided φ, gcd, and is_prime utilities
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t index)
{
    if (index == 1)
        return (seq_t)1;           /* the first term is 1 */

    idx_t count = 1;               /* we already have the term '1' */
    idx_t n = 2;

    for (;; ++n)
    {
        /* skip primes (prime numbers are not included in the sequence) */
        if (is_prime(n))
            continue;

        seq_t n_seq = (seq_t)n;
        seq_t phi_n = phi(n);              /* φ(n) */
        seq_t g = gcd(n_seq, phi_n);       /* gcd(n, φ(n)) */

        if (g == (seq_t)1)
        {
            ++count;
            if (count == index)
                return n_seq;
        }
    }
}
