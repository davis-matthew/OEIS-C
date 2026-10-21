/*
 * A221837  Number of integer Heron triangles of height n such that the angles
 *          adjacent to the base are not right.
 * https://oeis.org/A221837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A046079(n))^2 A046079(n) = number of integer right
 *            triangles with a leg of length n. Using divisor counts: if n is
 *            odd: A046079(n) = (tau(n²) - 1) / 2 if n is even: A046079(n) =
 *            (tau(n²/4) - 1) / 2 where tau(m) = number of positive divisors
 *            of m
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* factorisation of n */
    idx_t tmp = n;
    uint64_t odd_tau = 1;               /* product of (2*e_i+1) for odd primes */
    uint64_t e2 = 0;                    /* exponent of prime 2 */

    /* handle factor 2 */
    while ((tmp & 1ULL) == 0) {
        ++e2;
        tmp >>= 1;
    }

    /* handle odd primes */
    for (idx_t p = 3; (p * p) <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            odd_tau *= (2 * e + 1);
        }
    }
    if (tmp > 1) {                     /* remaining prime factor > sqrt(tmp) */
        odd_tau *= 3;                  /* e = 1 -> (2*1+1) = 3 */
    }

    /* compute A046079(n) */
    uint64_t A;
    if ((n & 1ULL) == 0) {             /* n even */
        /* tau(n²/4) = (2*e2 - 1) * odd_tau */
        uint64_t tau_n2_4 = (2 * e2 - 1) * odd_tau;
        A = (tau_n2_4 - 1) / 2;
    } else {                           /* n odd */
        /* tau(n²) = odd_tau */
        uint64_t tau_n2 = odd_tau;
        A = (tau_n2 - 1) / 2;
    }

    /* square the result */
    uint64_t ans = A * A;               /* A fits in uint64_t for all n considered */
    return (seq_t)ans;
}
