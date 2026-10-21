/*
 * A081396  Number of common prime factors (ignoring multiplicity) of sigma(n)
 *          = A000203(n) and phi(n) = A000010(n).
 * https://oeis.org/A081396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors common to σ(n) and φ(n)
 * Range:     u8 n <= 775, u16 n <= 9313, u32 n <= 9313, u64 n <= 9313,
 *            u128 n <= 9313, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the supplied utilities:
   - seq_t sigma_of(idx_t n): sum of divisors of n
   - seq_t phi(idx_t n): Euler's totient of n
   It factors σ(n) and φ(n) with trial division (works for 64‑bit values)
   and counts the primes that appear in both factorizations. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

/* --------------------------------------------------------------------- */
/* Factor a uint64_t into its prime bases and exponents by trial division. */
/* --------------------------------------------------------------------- */
static void factor_u64(uint64_t n, uint64_t primes[], int exps[], size_t *cnt)
{
    *cnt = 0;
    if (n <= 1u) return;

    /* handle factor 2 */
    if ((n & 1ULL) == 0) {
        int e = 0;
        while ((n & 1ULL) == 0u) {
            n >>= 1;
            ++e;
        }
        primes[(*cnt)]   = 2ULL;
        exps[(*cnt)]     = e;
        ++(*cnt);
    }

    /* trial division with odd candidates */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            int e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            primes[(*cnt)]   = p;
            exps[(*cnt)]     = e;
            ++(*cnt);
        }
    }

    if (n > 1ULL) {          /* remaining prime factor */
        primes[(*cnt)]   = n;
        exps[(*cnt)]     = 1;
        ++(*cnt);
    }
}

/* --------------------------------------------------------------------- */
/* Return count of distinct primes common to two uint64_t numbers.        */
/* --------------------------------------------------------------------- */
static unsigned int common_prime_factor_count(uint64_t a, uint64_t b)
{
    uint64_t primes_a[64], primes_b[64];
    int exps_a[64], exps_b[64];
    size_t cnt_a, cnt_b;

    factor_u64(a, primes_a, exps_a, &cnt_a);
    factor_u64(b, primes_b, exps_b, &cnt_b);

    unsigned int common = 0;
    for (size_t i = 0; i < cnt_a; ++i) {
        for (size_t j = 0; j < cnt_b; ++j) {
            if (primes_a[i] == primes_b[j]) {
                ++common;
                break;
            }
        }
    }
    return common;
}

/* --------------------------------------------------------------------- */
/* Main sequence function: number of distinct common prime factors       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is at least 1 by OEIS definition */
    seq_t sigma_n = sigma_of(n);
    seq_t phi_n   = phi(n);

    /* For n up to ~10^4 the values fit into 64 bits.  Convert safely. */
    uint64_t sigma64 = (uint64_t)sigma_n;
    uint64_t phi64   = (uint64_t)phi_n;

    unsigned int res = common_prime_factor_count(sigma64, phi64);
    return (seq_t)res;
}
