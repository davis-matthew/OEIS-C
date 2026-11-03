/*
 * A029936  Number of cusps of Gamma_1(n)\P_1(Q).
 * https://oeis.org/A029936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/2) * Sum_{d|n} phi(d)*phi(n/d) with a(1) = 1, a(2) =
 *            2, a(4) = 3 (all other n ≥ 5 use the formula)
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9713 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 4) return 3;

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t phi_d   = totient(d);
            seq_t phi_nd  = totient(n / d);
            seq_t prod    = phi_d * phi_nd;
            /* each divisor pair {d, n/d} contributes the same product twice
               except when d == n/d (perfect square case) */
            sum += prod;
            if (d != n / d) sum += prod; /* second occurrence */
        }
    }
    return sum / (seq_t)2;
}
