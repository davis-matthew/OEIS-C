/*
 * A035199  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          17.
 * https://oeis.org/A035199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(17,d) Multiplicative rule: if n =
 *            ∏p^e, then a(p^e) = 1 if p = 17 a(p^e) = e+1 if Jacobi(17,p) =
 *            +1 a(p^e) = (1+(-1)^e)/2 if Jacobi(17,p) = -1 For p = 2,
 *            Jacobi(17,2) = +1, so a(2^e) = e+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* modular exponentiation  (base^exp mod mod) */
static uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Legendre symbol (17|p) for an odd prime p; returns 1, -1 or 0. */
static int legendre17(uint64_t p)
{
    /* p is an odd prime, p ≠ 17 or may be 17 (returns 0) */
    uint64_t r = modpow_u64(17ULL, (p - 1) >> 1, p);
    if (r == 1) return 1;
    if (r == p - 1) return -1;
    return 0; /* occurs only when p | 17, i.e. p == 17 */
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t x = n;

    /* factor 2 first */
    idx_t e2 = 0;
    while ((x & 1ULL) == 0 && x > 0) {
        e2++;
        x >>= 1;
    }
    if (e2 > 0)
        result *= (seq_t)(e2 + 1);

    /* factor odd primes */
    idx_t p = 3;
    while (p * p <= x) {
        if ((x % p) == 0) {
            /* compute exponent of p */
            idx_t e = 0;
            while (x % p == 0) {
                e++;
                x /= p;
            }
            seq_t mult;
            if (p == 17ULL) {
                mult = 1;
            } else {
                int ls = legendre17(p);
                if (ls == 1) {
                    mult = (seq_t)(e + 1);
                } else { /* ls == -1 */
                    mult = ((e & 1ULL) == 0) ? 1 : 0;
                }
            }
            result *= mult;
        }
        p += 2;
    }

    /* any remaining prime factor > sqrt(original n) */
    if (x > 1) {
        /* x is prime, exponent = 1 */
        seq_t mult;
        if (x == 17ULL) {
            mult = 1;
        } else {
            int ls = legendre17(x);
            if (ls == 1) {
                mult = 2;          /* e = 1 → e+1 = 2 */
            } else { /* ls == -1 */
                mult = 0;          /* (1+(-1)^1)/2 = 0 */
            }
        }
        result *= mult;
    }

    return result;
}
