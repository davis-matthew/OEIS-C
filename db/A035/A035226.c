/*
 * A035226  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = 44.
 * https://oeis.org/A035226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(44,d) (Kronecker symbol)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9710 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Return the factor contributed by the prime power p^exp
   when Kronecker(44,p) = 1 or -1.  For Kronecker(44,p)=0 the
   factor is always 1 and is handled separately. */
static inline seq_t factor_for_prime_exp(uint64_t exp, int kronecker)
{
    if (kronecker == 1) {                /* a(p^e) = e+1 */
        return (seq_t)(exp + 1);
    } else { /* kronecker == -1 */        /* a(p^e) = (1+(-1)^e)/2 : 1 if e even, 0 if odd */
        return (seq_t)((exp % 2ULL) == 0 ? 1ULL : 0ULL);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety: sequence starts at offset 1 */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t exp;

    /* Factor out the prime 2 – Kronecker(44,2)=0 → factor 1 (do nothing) */
    exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }

    /* Factor out the prime 11 – Kronecker(44,11)=0 → factor 1 */
    exp = 0;
    while (m % 11ULL == 0) {
        m /= 11ULL;
        exp++;
    }

    /* Factor remaining odd primes up to sqrt(m) */
    for (uint64_t p = 3; m / p >= p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            int K = jacobi_symbol(44, p);      /* p is prime → K = ±1 */
            result *= factor_for_prime_exp(exp, K);
        }
    }

    /* If a prime > sqrt(n) remains, it appears with exponent 1 */
    if (m > 1) {
        int K = jacobi_symbol(44, m);          /* m is prime (odd, ≠ 11) */
        result *= factor_for_prime_exp(1, K);
    }

    return result;
}
