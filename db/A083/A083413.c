/*
 * A083413  a(n) = Sum_{d|n} d*2^(d-1) for n > 0.
 * https://oeis.org/A083413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*2^(d-1) (OEIS A083413)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* gives seq_t pow2(idx_t exp) */

/* Computes the nth term of OEIS A083413.
 * For n = 0, a(0) = 0 is returned.
 * For n > 0, a(n) = Σ_{d|n} d * 2^(d-1). */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t sum = 0;
    /* Enumerate all divisors d of n in pairs (d, n/d). */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t term_d = (seq_t)d * pow2(d - 1);
            sum += term_d;

            /* paired divisor n/d, if distinct from d */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t term_d2 = (seq_t)d2 * pow2(d2 - 1);
                sum += term_d2;
            }
        }
    }
    return sum;
}
