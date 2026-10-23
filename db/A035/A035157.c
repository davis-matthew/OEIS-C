/*
 * A035157  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -33.
 * https://oeis.org/A035157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-33,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------------------------------------------------------------------
 * Legendre symbol (−33/p) for a prime p that does not divide 33.
 * Uses binary exponentiation modulo p with 128‑bit multiplication
 * to avoid overflow.
 * ---------------------------------------------------------------------- */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1ULL) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static int legendre_neg33(uint64_t p)
{
    /* p is an odd prime, p not dividing 33 */
    uint64_t a = (p - (33ULL % p)) % p;          /* a ≡ −33 (mod p) */
    uint64_t r = pow_mod64(a, (p - 1ULL) >> 1ULL, p);
    if (r == p - 1ULL) return -1;
    return (int)r;      /* 0 or 1 */
}

/* ----------------------------------------------------------------------
 * Factorisation of an unsigned 64‑bit integer using trial division.
 * Fills arrays primes[] and exps[] (capacity 64) and writes the
 * number of distinct prime factors into *cnt.
 * ---------------------------------------------------------------------- */
static void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    uint64_t tmp = n;
    if (tmp % 2ULL == 0ULL) {
        uint64_t e = 0;
        while (tmp % 2ULL == 0ULL) {
            tmp /= 2ULL;
            ++e;
        }
        primes[c] = 2ULL;
        exps[c] = e;
        ++c;
    }
    for (uint64_t p = 3ULL; p * p <= tmp; p += 2ULL) {
        if (tmp % p == 0ULL) {
            uint64_t e = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (tmp > 1ULL) {                 /* remaining prime factor */
        primes[c] = tmp;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ----------------------------------------------------------------------
 * Main sequence routine
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* off‑offset, unused */
    uint64_t u = (uint64_t)n;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(u, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        int kr;   /* Kronecker(-33,p) */

        if (p == 2ULL) {
            kr = 1;                /* by definition of the Kronecker symbol */
        } else if (p == 3ULL || p == 11ULL) {
            kr = 0;                /* they divide 33 */
        } else {
            kr = legendre_neg33(p);
        }

        seq_t factor;
        if (kr == 0) {
            factor = 1;            /* p^e gives no contribution */
        } else if (kr == -1) {
            factor = (e & 1ULL) ? 0 : 1;   /* (1+(-1)^e)/2 */
        } else { /* kr == 1 */
            factor = (seq_t)(e + 1ULL);    /* e+1 */
        }

        result *= factor;
    }
    return result;
}
