/*
 * A035211  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          29.
 * https://oeis.org/A035211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Σ_{d|n} Kronecker(29,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p^e) = 1                                 if p = 29
 * a(p^e) = (1+(-1)^e)/2   if Kronecker(29,p) = –1
 * a(p^e) = e+1            if Kronecker(29,p) =  1   */

/* The function below is fully self‑contained and does not require the
 * external jacobi_symbol implementation.  It uses a small modular
 * exponentiation routine to compute the Legendre symbol (29/p)
 * for odd prime p.  For p = 2 and p = 29 the values are handled
 * explicitly.  The routine factors n by trial division, which is
 * fast enough for n up to at least 10 000 (the usual testing range
 * for OEIS sequences).  All arithmetic is performed on seq_t, the
 * unsigned integer type supplied by the build system. */

#include <stdint.h>

/* --------------------------------------------------------------------- */
/*                              Helper routines                         */
/* --------------------------------------------------------------------- */

/* modular multiplication – works safely for 64‑bit values                    */
static inline uint64_t modmul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation :  base^exp (mod mod)                            */
static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = modmul_uint64(result, base, mod);
        base = modmul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Legendre symbol (29/p).  For p = 2 we return –1.  For every odd prime p
 * we compute 29^((p‑1)/2) mod p, which is 1 or p–1.  The symbol is 1 if
 * the result is 1, –1 if the result is p–1, otherwise 0 (should not
 * occur for odd primes).                                                  */
static inline int legendre_29(uint64_t p)
{
    if (p == 2) return -1;                /* (29/2) = –1         */
    if (p == 29) return 1;                /* handled outside      */
    uint64_t mod = p;
    uint64_t exp = (p - 1) / 2;
    uint64_t base = 29 % mod;
    uint64_t val = modpow_uint64(base, exp, mod);
    if (val == 1) return 1;
    if (val == mod - 1) return -1;
    return 0;    /* should not happen for odd prime p */
}

/* return the multiplicative factor contributed by p^exp in a(n)            */
static inline seq_t mult_for_prime(uint64_t p, uint64_t exp)
{
    if (p == 29)                /* a(29^e) = 1 for all e ≥ 1 */
        return (seq_t)1;

    int kr = legendre_29(p);    /* –1, 0 or 1, but only –1 or 1 valid for odd p≠29 */

    if (kr == -1) {             /* (1+(-1)^exp)/2  */
        return (exp & 1) ? (seq_t)0 : (seq_t)1;
    }
    /* kr == 1  */
    return (seq_t)(exp + 1);
}

/* --------------------------------------------------------------------- */
/*                           Main sequence function                      */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence starts at index 1 */
        return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;     /* use native 64‑bit integer for factoring */

    /* Trial division up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            seq_t mult = mult_for_prime(p, exp);
            result *= mult;
            if (result == 0)          /* early exit */
                return 0;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t mult = mult_for_prime(m, 1);
        result *= mult;
    }

    return result;
}
