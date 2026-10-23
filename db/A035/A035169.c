/*
 * A035169  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -21.
 * https://oeis.org/A035169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-21, d) Multiplicative: a(1)=1. For p
 *            = 2: a(2^e) = (1+(-1)^e)/2 (Kronecker(-21,2) = -1) For odd p: s
 *            = Kronecker(-21, p) if s = 0 (p = 3 or 7) : a(p^e) = 1 if s = -1
 *            : a(p^e) = (1+(-1)^e)/2 if s = 1 : a(p^e) = e + 1 Result is the
 *            product over prime power factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* modular multiplication using 128-bit to avoid overflow */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Legendre symbol (a|p) for odd prime p, a may be negative.
   Returns 0 if a ≡ 0 mod p,
           1 if quadratic residue,
          -1 otherwise. */
static int legendre_modp(int64_t a, uint64_t p)
{
    if (p == 2) return 0;                 /* not used for odd primes */
    int64_t a_mod = a % (int64_t)p;
    if (a_mod < 0) a_mod += p;
    uint64_t am = (uint64_t)a_mod;
    if (am == 0) return 0;

    uint64_t exp = (p - 1) / 2;
    uint64_t base = am;
    uint64_t res = 1 % p;
    while (exp) {
        if (exp & 1) res = mod_mul(res, base, p);
        base = mod_mul(base, base, p);
        exp >>= 1;
    }
    return (res == p - 1) ? -1 : 1;
}

/* Kronecker(-21, p) for prime p. Handles p = 2 specially. */
static int kronecker_minus21_prime(uint64_t p)
{
    if (p == 2) {
        /* (-21 | 2) = -1 because -21 ≡ 3 (mod 8) */
        return -1;
    }
    /* if p divides 21, the symbol is 0 */
    if (p == 3 || p == 7) return 0;
    return legendre_modp(-21, p);
}

/* Multiplicative function a(n) based on the prime factorisation of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but return 0 */
    seq_t result = 1;

    uint64_t temp = (uint64_t)n;
    /* handle factor 2 separately */
    if ((temp & 1) == 0) {
        int e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            e++;
        }
        /* a(2^e) = (1+(-1)^e)/2 */
        if ((e & 1) != 0) return 0;          /* result becomes 0 */
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            int s = kronecker_minus21_prime(p);
            seq_t f;
            if (s == 0) {
                f = 1;
            } else if (s == -1) {
                f = ((e & 1) == 0) ? 1 : 0;
            } else { /* s == 1 */
                f = (seq_t)(e + 1);
            }
            result *= f;
            if (result == 0) return 0;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        int s = kronecker_minus21_prime(temp);
        seq_t f;
        if (s == 0) {
            f = 1;
        } else if (s == -1) {
            f = 1;           /* exponent is 1 => even? no, (-1)^1 = -1 => f=0?  */
            f = 0;           /* so we set direct */
        } else { /* s == 1 */
            f = 2;           /* e=1 -> e+1 = 2 */
        }
        /* correct for s == -1 case */
        if (s == -1) f = 0;
        result *= f;
    }

    return result;
}
