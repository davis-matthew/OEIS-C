/*
 * A038917  Primes p such that 39 is a square mod p.
 * https://oeis.org/A038917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that (39|p)=1 (including p|39)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_mul.h"

static bool is_quad_res(uint64_t p)
{
    /* p=2,3,13 are trivial and square */
    if (p == 2 || p == 3 || p == 13) return true;
    /* If p divides 39, 39≡0 (mod p) which is a perfect square (0) */
    if (p % 39 == 0) return true;

    /* Legendre symbol (39|p) = 39^((p-1)/2) mod p.
       It equals 1 for a quadratic residue, p-1 for a non-residue, and 0 if p|39. */
    uint64_t exp = (p - 1) >> 1;       /* (p-1)/2 */
    uint64_t res = 1 % p;
    uint64_t base = 39 % p;

    while (exp)
    {
        if (exp & 1) res = mod_mul(res, base, p);
        exp >>= 1;
        if (exp) base = mod_mul(base, base, p);
    }
    return (res == 0) || (res == 1);
}

static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 as a safe value */

    uint64_t candidate = 2;
    idx_t     found = 0;

    while (true)
    {
        if (is_prime_candidate(candidate))
        {
            if (is_quad_res(candidate))
            {
                ++found;
                if (found == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
