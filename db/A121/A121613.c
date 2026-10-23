/*
 * A121613  Expansion of psi(-x)^4 in powers of x where psi() is a Ramanujan
 *          theta function.
 * https://oeis.org/A121613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = b(2*n + 1) where b is multiplicative, defined by *
 *            b(p^e) = (p^(e+1)-1)/(p-1) if p ≡ 1 (mod 4) * = (-1)^e *
 *            (p^(e+1)-1)/(p-1) if p ≡ 3 (mod 4) * (for the trivial case
 *            b(1)=1). Since 2*n+1 is odd, only odd primes * appear
 * Range:     i8 n <= 45, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the contribution of a prime power p^e to the multiplicative
 * function b.  seq_t is the output type, p is an 64‑bit unsigned integer
 * and e is the exponent (positive).  The result is computed using
 * repeated multiplication so that it works for arbitrary precision
 * types that support the usual arithmetic operators. */
static seq_t prime_power_contrib(uint64_t p, unsigned int e)
{
    seq_t term = 1;   /* p^0 */
    seq_t sum  = 1;   /* Σ_{k=0..e} p^k starts with 1 */

    for (unsigned int k = 1; k <= e; ++k) {
        term = term * (seq_t)p;  /* term = p^k */
        sum  = sum  + term;     /* add it to the geometric sum */
    }

    /* If p ≡ 3 (mod 4) and e is odd, multiply by (-1)^e = -1. */
    if (((p >> 1) & 1U) && (e & 1U)) {   /* p mod 4 == 3 */
        sum = -sum;
    }

    return sum;
}

/* Factor a 64‑bit unsigned integer n into its prime powers.
 * The function writes up to *cnt primes and their exponents into the
 * supplied arrays primes[] and exps[].  At most 64 distinct prime
 * divisors can exist for a 64‑bit number, which is safe. */
static void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t index = 0;
    uint64_t remaining = n;

    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            uint64_t e = 0;
            do {
                remaining /= p;
                ++e;
            } while (remaining % p == 0);
            primes[index] = p;
            exps[index]   = e;
            ++index;
        }
    }
    if (remaining > 1) {
        primes[index] = remaining;
        exps[index]   = 1;
        ++index;
    }
    *cnt = index;
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    /* 2*n + 1 is always odd. */
    uint64_t m = 1ULL + 2ULL * (uint64_t)n;
    if (m == 1ULL) return (seq_t)1;   /* b(1) = 1 */

    /* Factor m.  The number of distinct prime factors of a 64‑bit
     * integer cannot exceed 64, so this buffer is safe. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   count = 0;
    factor_u64(m, primes, exps, &count);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        /* Skip the prime 2, but m is odd so this never occurs. */
        if (primes[i] == 2ULL) continue;
        prod = prod * prime_power_contrib(primes[i], (unsigned int)exps[i]);
    }

    return prod;
}
