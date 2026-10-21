/*
 * A087801  Greatest common divisor of tau(n)+sigma(n) and tau(n)*sigma(n),
 *          where tau = A000005 and sigma = A000203.
 * https://oeis.org/A087801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(tau(n)+sigma(n), tau(n)*sigma(n))
 * Range:     u8 n <= 389, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple Euclidean algorithm for 64‑bit integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Compute a(n) for n >= 1.  Returns 0 for n == 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but safe */

    uint64_t tau   = 1;
    uint64_t sigma = 1;
    uint64_t m     = (uint64_t)n;

    /* factor n by trial division */
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p != 0)
            continue;

        int exp = 0;
        uint64_t pe = 1;       /* p^exp */
        uint64_t sigma_p = 1;  /* sum of powers for this prime */

        while (m % p == 0) {
            m /= p;
            exp++;
            pe *= p;
            sigma_p += pe;
        }

        tau   *= (exp + 1);
        sigma *= sigma_p;
    }

    if (m > 1) { /* remaining prime factor */
        tau   *= 2;
        sigma *= (1 + m);
    }

    uint64_t sum = tau + sigma;
    uint64_t prod = tau * sigma;
    uint64_t g = gcd_uint64(sum, prod);

    return (seq_t)g;
}
