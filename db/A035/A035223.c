/*
 * A035223  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          41.
 * https://oeis.org/A035223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(41,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the multiplicative definition:
   a(p^e) = e+1                  if (41/p) = 1
   a(p^e) = (1+(-1)^e)/2        if (41/p) = -1
   a(p^e) = 1                    if p = 41
   a(1)   = 1
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* fast modular exponentiation for unsigned 64-bit integers */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = mod == 1 ? 0 : 1;
    base %= mod;
    while (exp) {
        if (exp & 1u) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

/* Jacobi symbol (41/p) for odd prime p (p != 41). Returns 1, -1, or 0 (only for p==41). */
static int symbol_41(uint64_t p)
{
    if (p == 2u) return 1;          /* 41 ≡ 1 (mod 8) */
    if (p == 41u) return 0;         /* (41/41) = 0 */
    uint64_t exp = (p - 1u) >> 1u;   /* (p-1)/2 */
    uint64_t val = modpow_uint64(41u % p, exp, p);
    return (val == 1u) ? 1 : -1;     /* p is prime, so val is 1 or p-1 */
}

/* Multiplicative computation of the sequence value a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; return 0 for safety */

    seq_t result = 1;
    uint64_t tmp   = (uint64_t)n;  /* local copy for factorisation */

    /* factor 2 separately */
    if (tmp % 2u == 0u) {
        size_t e = 0u;
        while ((tmp & 1u) == 0u) {
            tmp >>= 1u;
            ++e;
        }
        /* (41/2) = +1 */
        seq_t fac = (seq_t)e + 1u;
        result *= fac;
    }

    /* factor odd primes */
    for (uint64_t p = 3u; p * p <= tmp; p += 2u) {
        if (tmp % p == 0u) {
            size_t e = 0u;
            while (tmp % p == 0u) {
                tmp /= p;
                ++e;
            }
            int s = symbol_41(p);
            seq_t fac;
            if (s == 0)        fac = 1;                 /* p == 41 → factor 1   */
            else if (s == 1)   fac = (seq_t)e + 1u;      /* (41/p) = 1           */
            else               fac = (e & 1u) ? 0u : 1u; // (41/p) = -1
            result *= fac;
        }
    }

    /* remaining prime factor (exponent 1) */
    if (tmp > 1u) {
        uint64_t p = tmp;
        int s = symbol_41(p);
        seq_t fac;
        if (s == 0)        fac = 1;                 /* p == 41 */
        else if (s == 1)   fac = 2u;                /* (41/p)=1, e=1 → e+1 */
        else               fac = 0u;                /* (41/p)=-1, e=1 → 0 */
        result *= fac;
    }

    return result;
}
