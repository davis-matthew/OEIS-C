/*
 * A109395  Denominator of phi(n)/n = Product_{p|n} (1 - 1/p);
 *          phi(n)=A000010(n), the Euler totient function.
 * https://oeis.org/A109395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, phi(n))
 * Range:     u8 n <= 255, u16 n <= 8689, u32 n <= 8689, u64 n <= 8689,
 *            u128 n <= 8689, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi(n) is Euler's totient function. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1, as per OEIS offset */
    uint64_t N = (uint64_t)n;
    uint64_t phi = N;
    uint64_t tmp = N;

    /* factorise N: for each distinct prime p | N */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            /* remove p completely from tmp */
            while (tmp % p == 0) tmp /= p;
            /* update phi: phi = phi / p * (p-1) */
            phi = phi / p * (p - 1);
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        phi = phi / tmp * (tmp - 1);
    }

    /* compute gcd(N, phi) */
    uint64_t a_val = N, b_val = phi;
    while (b_val != 0) {
        uint64_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    uint64_t g = a_val;

    /* return n / g in the target seq_t type */
    return (seq_t)((seq_t)n / (seq_t)g);
}
