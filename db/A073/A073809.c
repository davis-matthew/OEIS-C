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
 * Range:     u8 n <= 9752, u16 n <= 9752, u32 n <= 9752, u64 n <= 9752,
 *            u128 n <= 9752, big any n
 * Helpers:   sigma1_sigma3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma1_sigma3.h"

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
