/*
 * A006956  Denominator of (2n+1)(2n+2) B_{2n}, where B_n are the Bernoulli
 *          numbers. Also denominators of the asymptotic expansion of the
 *          polygamma function psi'''(z).
 * https://oeis.org/A006956
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (2k+1)(2k+2)*B_{2k} with k = n-4, a(3) =
 *            a(4) = 1. By von Staudt-Clausen: a(n) = product of primes p with
 *            (p-1) | 2k and p not dividing (2k+1)(2k+2)
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 183, u64 n <= 543,
 *            u128 n <= 1000, big any n
 * Helpers:   a006956_isprime, a006956_try
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a006956_isprime.h"
#include "../utils/a006956_try.h"

seq_t a(idx_t n) {
    uint64_t k, m, d;
    seq_t res = 1;
    if (n < 5) return 1;
    k = (uint64_t)n - 4;
    m = 2 * k;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            a006956_try(d + 1, m, &res);
            if (e != d) a006956_try(e + 1, m, &res);
        }
    }
    return res;
}
