/*
 * A175926  Sum of divisors of cubes.
 * https://oeis.org/A175926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of divisors of n^3, i.e. a(n) = sigma(n^3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 718, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes σ(n³) = ∑_{d|n³} d.  The function works for any n ≥ 1,
   using only integer arithmetic supported by seq_t/idx_t.  It is
   independent of external utilities; however it can also use
   helper routines from the build as they are available.  The
   implementation uses a fast binary exponentiation for small
   exponents and a simple trial division to factor n.  The code
   stays well under 2 s for n up to about 10 000.  */

#include <stdint.h>
#include "../utils/pow_seq_local.h"

/* local exponentiation: base^exp for small exp (exp fits in int) */

seq_t a(idx_t n)
{
    /* Work with a 64‑bit copy of the index; n is assumed to fit. */
    uint64_t m = (uint64_t)n;
    if (n == 0) return 0;                /* not used in OEIS offset 1 */
    if (m == 1) return 1;                /* a(1) = 1 */

    seq_t result = 1;

    /* handle factor 2 separately */
    if ((m & 1) == 0) {
        int e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            e++;
        }
        seq_t pow1 = pow_seq_local((seq_t)2, 3 * e + 1);
        seq_t term = (pow1 - 1) / (2 - 1);
        result *= term;
    }

    /* odd primes up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t pow1 = pow_seq_local((seq_t)p, 3 * e + 1);
            seq_t term = (pow1 - 1) / (p - 1);
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* its exponent is 1 */
        seq_t pow1 = pow_seq_local((seq_t)m, 4);  /* 3*1+1 = 4 */
        seq_t term = (pow1 - 1) / (m - 1);
        result *= term;
    }

    return result;
}
