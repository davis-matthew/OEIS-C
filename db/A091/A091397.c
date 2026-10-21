/*
 * A091397  a(n) = Product_{ p | n } (1 + Legendre(3,p) ).
 * https://oeis.org/A091397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p|n} (1 + Legendre(3,p))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n has the prime 2 as a divisor then a(n)=0, since (1+Legendre(3,2))=0. */
/* If n has any odd prime p with Legendre(3,p)=-1 then a(n)=0. */
/* Otherwise a(n)=2^{k} where k is the number of odd prime divisors of n
   (excluding 3) for which Legendre(3,p)=+1. The prime 3 contributes 1 to the product. */

#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation for uint64_t using 128‑bit multiplication */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1u)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Even numbers immediately give zero due to the factor p = 2 */
    if ((n & 1u) == 0u)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    seq_t prod = (seq_t)1;

    /* Trial division for distinct prime factors up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            /* distinct prime factor found */
            if (p != 3) {
                uint64_t leg = modpow64(3, (p - 1) / 2, p);
                if (leg == 1) {
                    prod *= (seq_t)2;
                } else {                  /* leg == p-1 → factor 0 */
                    return (seq_t)0;
                }
            }
            /* Remove all occurrences of this prime */
            while (m % p == 0)
                m /= p;
        }
    }

    /* Remaining factor > sqrt(original n) (if any) */
    if (m > 1) {
        uint64_t p = m;
        if (p != 3) {
            uint64_t leg = modpow64(3, (p - 1) / 2, p);
            if (leg == 1) {
                prod *= (seq_t)2;
            } else {
                return (seq_t)0;
            }
        }
    }

    return prod;
}
