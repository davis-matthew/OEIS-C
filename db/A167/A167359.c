/*
 * A167359  Totally multiplicative sequence with a(p) = (p+2)*(p-3) = p^2-p-6
 *          for prime p.
 * https://oeis.org/A167359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Π_{p^e‖n} ((p+2)*(p-3))^e ; if 3 | n then a(n)=0
 * Range:     i8 n <= 12, i16 n <= 187, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A167359. */
/* Uses the helper pow_int (../utils/pow_int.h). */
#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;                 /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p == 3) return 0;         /* any factor 3 gives zero */
            seq_t factor = (seq_t)p * (seq_t)p - (seq_t)p - (seq_t)6;
            result *= pow_int(factor, (idx_t)e);
        }
    }

    if (m > 1) {                           /* remaining prime factor */
        uint64_t p = m;
        if (p == 3) return 0;
        seq_t factor = (seq_t)p * (seq_t)p - (seq_t)p - (seq_t)6;
        result *= factor;                  /* exponent 1 */
    }

    return result;
}
