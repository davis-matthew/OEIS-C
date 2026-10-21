/*
 * A097011  Remainder of sigma(n) modulo 30.
 * https://oeis.org/A097011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) mod 30
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the divisors of n (σ(n)) and returns it mod 30. */
#include <stdint.h>

/* Helper that computes σ(n) for a 64‑bit integer using trial division. */
static uint64_t sigma_u64(uint64_t n)
{
    uint64_t sum = 1;
    /* Handle factor 2 separately */
    if (n % 2 == 0) {
        uint64_t term = 1;
        uint64_t pw = 1;
        while (n % 2 == 0) {
            n /= 2;
            pw *= 2;
            term += pw;
        }
        sum *= term;
    }
    /* Odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t term = 1;
            uint64_t pw = 1;
            while (n % p == 0) {
                n /= p;
                pw *= p;
                term += pw;
            }
            sum *= term;
        }
    }
    /* If remaining n is prime > 1 */
    if (n > 1) {
        sum *= (1 + n);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute σ(n) and return it modulo 30. */
    uint64_t sigma = sigma_u64((uint64_t)n);
    return (seq_t)(sigma % 30);
}
