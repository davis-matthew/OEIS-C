/*
 * A073809  Number of common divisors of sigma_1(n) and sigma_3(n).
 * https://oeis.org/A073809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #divisors( gcd( σ₁(n), σ₃(n) ) ) where σ₁(n) is the sum
 *            of divisors of n and σ₃(n) is the sum of cubes of divisors. The
 *            function implements this definition by factoring *n*, evaluating
 *            the two σ‑values, taking their GCD, then counting the divisors
 *            of that GCD. All work is done with 64‑bit...
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9752 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute σ₁(n) and σ₃(n) for a given unsigned integer *n*.
   The results are returned in *sigma1* and *sigma3* (both 64‑bit). */
static void sigma1_sigma3(uint64_t n, uint64_t *sigma1, uint64_t *sigma3) {
    unsigned __int128 s1 = 1;
    unsigned __int128 s3 = 1;
    uint64_t tmp = n;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p != 0) continue;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }

        /* term for σ₁ :  (p^(exp+1)-1)/(p-1) */
        unsigned __int128 p_pow = 1;
        for (uint64_t i = 0; i < exp + 1; ++i) p_pow *= p;
        uint64_t term1 = (uint64_t)((p_pow - 1) / (p - 1)) & 0xFFFFFFFFFFFFFFFFull;
        s1 *= term1;

        /* term for σ₃ :  (p^(3(exp+1))-1)/(p³-1) */
        unsigned __int128 p3_pow = 1;
        for (uint64_t i = 0; i < 3 * (exp + 1); ++i) p3_pow *= p;
        uint64_t denom = (p * p * p) - 1;
        uint64_t term3 = (uint64_t)((p3_pow - 1) / denom) & 0xFFFFFFFFFFFFFFFFull;
        s3 *= term3;
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        uint64_t p = tmp;
        uint64_t exp = 1;                  /* exponent is 1 */

        unsigned __int128 p_pow = 1;
        for (uint64_t i = 0; i < exp + 1; ++i) p_pow *= p;
        uint64_t term1 = (uint64_t)((p_pow - 1) / (p - 1));
        s1 *= term1;

        unsigned __int128 p3_pow = 1;
        for (uint64_t i = 0; i < 3 * (exp + 1); ++i) p3_pow *= p;
        uint64_t denom = (p * p * p) - 1;
        uint64_t term3 = (uint64_t)((p3_pow - 1) / denom);
        s3 *= term3;
    }

    *sigma1 = (uint64_t)s1;
    *sigma3 = (uint64_t)s3;
}

/* ------------------------------------------------------------ */
/* Helper: GCD of two 64‑bit unsigned integers.                 */
static uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Factor a 64‑bit integer *n* and count its divisors. Return the
   number of positive divisors of *n*. */
static uint64_t divisor_count(uint64_t n) {
    uint64_t count = 1;
    uint64_t tmp = n;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p != 0) continue;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        count *= (exp + 1);
    }

    if (tmp > 1)            /* remaining prime factor */
        count *= 2;

    return count;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset‑1 behaviour is undefined */

    uint64_t sigma1, sigma3;
    sigma1_sigma3((uint64_t)n, &sigma1, &sigma3);

    uint64_t g = gcd64(sigma1, sigma3);

    uint64_t tau = divisor_count(g);

    return (seq_t)tau;
}
