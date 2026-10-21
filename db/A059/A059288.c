/*
 * A059288  a(n) = binomial(2*n,n) mod n.
 * https://oeis.org/A059288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n) mod n
 * Range:     u8 n <= 305, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external integer types defined by the build system */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 0U || n == 1U)
        return (seq_t)0;

    /* modulus for the computation (use a native type to avoid overflow) */
    uint64_t mod = (uint64_t)n;          /* n is < 2^16 for the expected limits */

    /* sieve primes up to 2*n ------------------------------------------------ */
    idx_t limit = 2U * n;
    size_t sz = (size_t)limit + 1U;       /* array for primality flags */

    uint8_t *is_composite = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_composite)
        return (seq_t)0;                  /* memory allocation failure */

    /* storage for the primes */
    uint32_t *primes = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!primes) {
        free(is_composite);
        return (seq_t)0;
    }

    size_t primes_cnt = 0U;
    for (idx_t i = 2U; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i;
                     j <= (uint64_t)limit; j += (uint64_t)i)
                    is_composite[(size_t)j] = 1U;
            }
        }
    }

    /* compute binomial(2*n,n) modulo n using prime exponents ------------- */
    uint64_t result = 1ULL;

    for (size_t pi = 0U; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        uint64_t exp = 0ULL;

        /* exponent of p in (2*n)! minus 2 times exponent of p in n! */
        uint64_t pp = (uint64_t)p;
        while (pp <= (uint64_t)limit) {
            uint64_t term1 = (uint64_t)limit / pp;  /* floor(2n / pp) */
            uint64_t term2 = (uint64_t)n / pp;       /* floor(n / pp)  */
            exp += term1 - 2ULL * term2;
            if (pp > UINT64_MAX / (uint64_t)p)
                break;
            pp *= (uint64_t)p;
        }
        if (exp == 0ULL)
            continue;

        /* fast modular exponentiation: p^exp mod n */
        uint64_t base   = (uint64_t)p % mod;
        uint64_t power  = 1ULL;
        uint64_t e      = exp;
        while (e) {
            if (e & 1ULL)
                power = (power * base) % mod;
            e >>= 1ULL;
            if (e)
                base = (base * base) % mod;
        }

        result = (result * power) % mod;
    }

    free(is_composite);
    free(primes);

    return (seq_t)result;
}
