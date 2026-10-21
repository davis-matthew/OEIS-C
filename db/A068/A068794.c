/*
 * A068794  In prime factorization of n replace all primes with the least
 *          prime factor of n; a(1)=1.
 * https://oeis.org/A068794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A020639(n)^A001222(n) (lpf(n) ^ bigomega(n))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A068794(n) for n >= 1. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;   /* a(1) is defined to be 1 */

    idx_t m = n;        /* working copy of n                */
    idx_t lpf = 0;      /* least prime factor of n          */
    idx_t omega = 0;    /* number of prime factors (with rep.) */

    /* factor 2 */
    if ((m & 1) == 0) {
        lpf = 2;
        while ((m & 1) == 0) {
            m >>= 1;      /* m /= 2; */
            omega++;
        }
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (lpf == 0) lpf = p;
            while (m % p == 0) {
                m /= p;
                omega++;
            }
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        if (lpf == 0) lpf = m;
        omega++;
    }

    /* compute lpf^omega in seq_t arithmetic */
    seq_t result = 1;
    seq_t base = (seq_t)lpf;
    while (omega > 0) {
        if (omega & 1) result *= base;
        base *= base;
        omega >>= 1;
    }
    return result;
}
