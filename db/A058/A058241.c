/*
 * A058241  Number of sets of n positive integers that can be placed along a
 *          circle such that the set of sums of adjacent integers forms { 1,
 *          2, ..., n^2-n+1 }.
 * https://oeis.org/A058241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = φ(n^2 - n + 1) / (6 e) if n-1 = p^e (p prime, e > 0); *
 *            a(1) = a(2) = 1, otherwise 0. * This implements that formula
 *            directly
 * Range:     u8 n <= 16, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n <= 2) return (seq_t)1;

    /* ---- Check whether n-1 is a prime power --------------------- */
    uint64_t m = (uint64_t)(n - 1);        /*  m >= 2 here            */
    uint64_t base = 0;                     /* candidate prime p       */
    uint64_t exp  = 0;                     /* exponent e              */

    /* Find the smallest divisor of m */
    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            base = i;
            break;
        }
    }

    if (base == 0) { /* m itself is prime */
        base = m;
        exp  = 1;
    } else {
        /* Count the exponent of the found prime */
        uint64_t tmp = m;
        while (tmp % base == 0) {
            tmp /= base;
            ++exp;
        }
        /* If a second distinct factor remains, m is not a prime power */
        if (tmp > 1) return (seq_t)0;
    }

    /* ---- Compute φ(n^2 - n + 1) ---------------------------------- */
    seq_t y = (seq_t)n * (seq_t)n - (seq_t)n + (seq_t)1; /* n^2 - n + 1   */
    seq_t phi = y;
    seq_t temp = y;

    for (uint64_t i = 2; i <= (uint64_t)(temp / i); ++i) {
        if (temp % (seq_t)i == 0) {
            phi = phi / (seq_t)i * ((seq_t)i - (seq_t)1);
            while (temp % (seq_t)i == 0) {
                temp /= (seq_t)i;
            }
        }
    }
    if (temp > (seq_t)1) {
        /* Remaining prime factor */
        phi = phi / temp * (temp - (seq_t)1);
    }

    /* ---- Final division ------------------------------------------- */
    seq_t denom = (seq_t)(6 * exp);
    return phi / denom;
}
