/*
 * A035197  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          15.
 * https://oeis.org/A035197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(15,d) Multiplicative: for prime power
 *            p^e - if p = 3 or 5 (Kronecker(15,p)=0) : a(p^e)=1 - if p odd
 *            prime with Kronecker(15,p)=-1 : a(p^e) = 1 if e even, 0 if e odd
 *            - if p != 2,3,5 and Kronecker(15,p)=1 : a(p^e)=e+1 - p = 2 :
 *            Kronecker(15,2)=1 so a(2^e)=e+1 Thus a(n) is...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* -------------- local helper functions ---------------- */

/* exponentiation modulo (using 128‑bit to avoid overflow) */
static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Legendre symbol (a|p) for odd prime p. Returns -1,0,1. */
static inline int legendre_symbol(uint64_t a, uint64_t p)
{
    uint64_t r = mod_pow64(a % p, (p - 1) >> 1, p);
    if (r == 1ULL)
        return 1;
    if (r == p - 1)
        return -1;
    return 0;          /* this happens only when p divides a */
}

/* Jacobi symbol (15|p) for odd prime p.
   Because 15 = 3 * 5 and both primes differ from 2, we can use
   multiplicativity:   (15|p) = (3|p)*(5|p).               */
static inline int j15_over_p(uint64_t p)
{
    /* p != 3 and p != 5 because those cases are handled separately */
    int s3 = legendre_symbol(3, p);
    int s5 = legendre_symbol(5, p);
    return s3 * s5;   /* product is -1, 0, or 1 */
}

/* ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for index 0 – return 0 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;            /* work in native 64‑bit */

    while (m > 1) {
        uint64_t p = 0;   /* smallest prime factor */
        uint64_t e = 0;   /* its exponent */

        /* trial division to find a prime factor of m */
        for (uint64_t f = 2; f * f <= m; ++f) {
            if (m % f == 0) {
                p = f;
                break;
            }
        }
        if (p == 0)           /* m itself is prime */
            p = m;

        /* count exponent of p */
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* determine Kronecker(15,p) */
        int k;   /* -1, 0, or 1 */
        if (p == 2) {
            k = 1;                           /* (15|2)=1 */
        } else if (p == 3 || p == 5) {
            k = 0;                           /* factor 3 or 5 -> 0 */
        } else {
            k = j15_over_p(p);               /* use Jacobi symbol */
        }

        /* apply multiplicative rule */
        if (k == 0) {
            /* value remains unchanged (factor *1) */
        } else if (k == -1) {
            if (e & 1ULL)              /* odd exponent gives zero factor */
                return (seq_t)0;
            /* even exponent gives factor 1 – nothing to change */
        } else /* k == 1 */ {
            result *= (seq_t)(e + 1);
        }
    }

    return result;
}
