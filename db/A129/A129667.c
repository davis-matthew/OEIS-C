/*
 * A129667  Dirichlet inverse of the Abelian group count (A000688).
 * https://oeis.org/A129667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, sign
 * Formula:   a(n) = Multiplicative function from the OEIS A129667 definition
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pentagonal_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by prime factorisation: for each prime power p^e,
   the value is (-1)^m if e = m(3m-1)/2 or e = m(3m+1)/2, otherwise 0. */

#include <stdint.h>
#include "../utils/pentagonal_val.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;           /* a(1) = 1 */

    idx_t m = n;
    seq_t result = 1;

    /* Handle factor 2 separately. */
    if ((m & 1) == 0) {
        uint64_t exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++exp;
        }
        seq_t val = pentagonal_val(exp);
        if (val == 0) return 0;
        result *= val;
    }

    /* Trial division by odd primes up to sqrt(m). */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t val = pentagonal_val(exp);
            if (val == 0) return 0;
            result *= val;
        }
    }

    /* Remaining factor is prime with exponent 1. */
    if (m > 1) {
        seq_t val = pentagonal_val(1);
        if (val == 0) return 0;
        result *= val;
    }

    return result;
}
