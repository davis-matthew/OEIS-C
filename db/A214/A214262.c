/*
 * A214262  Expansion of eta(q)^5 * eta(q^3) * eta(q^6)^4 / eta(q^2)^4 in
 *          powers of q.
 * https://oeis.org/A214262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, changed
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 161, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 63 terms through n = 1974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A214262
 *  Multiplicative function a(n):
 *  a(1) = 1
 *  For prime power p^e:
 *     a(2^e)   = -(4^(e+1) + 9*(-1)^(e+1)) / 5
 *     a(3^e)   = 9^e
 *     a(p^e)   = ((p^2)^(e+1) – 1) / (p^2 – 1)   if p ≡ 1 (mod 6)
 *     a(p^e)   = ((p^2)^(e+1) – (-1)^(e+1)) / (p^2 + 1)  if p ≡ 5 (mod 6)
 *
 *  This implementation factors n by trial division (sufficient for n ≤ 10 000)
 *  and evaluates the product of the local factors using 64‑bit arithmetic.
 *  The final result is cast to seq_t.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the local factor for prime power p^e as a 64‑bit signed integer. */
static inline long long local_value_ll(uint64_t p, uint64_t e)
{
    /* Special primes 2 and 3 */
    if (p == 2) {
        long long pow4 = 1;
        for (uint64_t i = 0; i < e + 1; ++i) pow4 *= 4LL;
        long long alt = ((e + 1) & 1ULL) ? -1 : 1;
        long long numerator = pow4 + 9 * alt;
        return -numerator / 5;
    }
    if (p == 3) {
        long long pow9 = 1;
        for (uint64_t i = 0; i < e; ++i) pow9 *= 9LL;
        return pow9;
    }

    /* General primes */
    long long p2 = (long long)p * (long long)p;   /* p^2 */
    long long power = 1;
    for (uint64_t i = 0; i < e + 1; ++i) power *= p2;  /* (p^2)^(e+1) */

    if (p % 6ULL == 1ULL) {
        return (power - 1) / (p2 - 1);
    } else {   /* p % 6 == 5 */
        long long alt = ((e + 1) & 1ULL) ? -1 : 1;
        return (power - alt) / (p2 + 1);
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t nn = (uint64_t)n;
    long long result = 1;

    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            result *= local_value_ll(p, e);
        }
    }
    if (nn > 1) {                     /* remaining prime factor */
        result *= local_value_ll(nn, 1);
    }
    return (seq_t)result;
}
