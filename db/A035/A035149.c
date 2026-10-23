/*
 * A035149  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -41.
 * https://oeis.org/A035149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-41,d) The implementation factors n
 *            and multiplies the contributions of each prime factor according
 *            to the formulas above
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Jacobi symbol (a/n) for odd n>0.  Result is 0, 1, or -1.  */
static int jacobi_symbol_uint64(uint64_t a, uint64_t n)
{
    int result = 1;
    a %= n;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t nm = n & 7ULL;
            if (nm == 3ULL || nm == 5ULL)
                result = -result;
        }
        uint64_t tmp = a; a = n; n = tmp;
        if ((a & 3ULL) == 3ULL && (n & 3ULL) == 3ULL)
            result = -result;
        a %= n;
    }
    return (n == 1ULL) ? result : 0;
}

/* Kronecker symbol (−41/p) for a prime p>1.  Returns 0 at p=41,
   otherwise 1 or -1.  (−41/2) = 1.  */
static int kronecker_minus41_prime(uint64_t p)
{
    if (p == 41ULL)
        return 0;
    if (p == 2ULL)
        return 1; /* (−41/2) = 1 */
    int leg = jacobi_symbol_uint64(41ULL, p);   /* (41/p) */
    if (leg == 0)
        return 0;
    int neg1 = ((p >> 1) & 1ULL) ? -1 : 1;      /* (−1)^((p−1)/2) */
    return leg * neg1;
}

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but harmless */

    /* Work with a mutable copy of n. */
    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Handle prime factor 2 separately. */
    if (m % 2ULL == 0) {
        uint64_t exp = 0;
        while (m % 2ULL == 0) {
            m /= 2ULL;
            ++exp;
        }
        seq_t local;
        int kr = kronecker_minus41_prime(2ULL);  /* 1 */
        /* For p == 2, kr is 1, so use the "a(p^e) = e+1" rule. */
        local = (seq_t)(exp + 1ULL);
        result *= local;
    }

    /* Trial division for odd primes up to sqrt(m). */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t local;
            if (p == 41ULL) {
                local = 1;          /* a(41^e) = 1 for all e≥1 */
            } else {
                int kr = kronecker_minus41_prime(p);
                if (kr == -1) {
                    /* (1 + (-1)^e)/2 */
                    local = (exp & 1ULL) ? 0 : 1;
                } else { /* kr == 1 */
                    local = (seq_t)(exp + 1ULL);
                }
            }
            result *= local;
        }
    }

    /* Remaining factor, if any, is a prime > sqrt(original m). */
    if (m > 1ULL) {
        uint64_t p = m;
        uint64_t exp = 1;
        seq_t local;
        if (p == 41ULL) {
            local = 1;
        } else {
            int kr = kronecker_minus41_prime(p);
            if (kr == -1) {
                local = 0;          /* (1 + (-1)^1)/2 = 0 */
            } else {
                local = (seq_t)(exp + 1ULL); /* e + 1 = 2 */
            }
        }
        result *= local;
    }

    return result;
}
