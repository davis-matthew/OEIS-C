/*
 * A224914  Partial sums of A217854.
 * https://oeis.org/A224914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1..n} (-i)^tau(i)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 59, i128 n <= 179,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 9717 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   tau(i) = number of positive divisors of i */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* Pre‑compute sequence values up to n.  For small n (≤10000) a
       straightforward O(n√n) algorithm is sufficiently fast. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute τ(i) (number of divisors). */
        uint32_t tau = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if ((i % d) == 0) {
                if (d * d == i)
                    ++tau;
                else
                    tau += 2;
            }
        }

        /* Compute term = (-i)^τ(i). */
        seq_t term = ipow((seq_t)i, (seq_t)tau);
        if (tau & 1) /* odd exponent → negative sign */
            term = -term;

        sum += term;
    }

    return sum;
}
