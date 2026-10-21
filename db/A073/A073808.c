/*
 * A073808  Number of common divisors of sigma_1(n) and sigma_2(n).
 * https://oeis.org/A073808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of common divisors of sigma_1(n) and sigma_2(n)
 * Range:     u8 n <= 9583, u16 n <= 9583, u32 n <= 9583, u64 n <= 9583,
 *            u128 n <= 9583, big any n
 * Helpers:   _divisor_count_u64, _factor_u64, _gcd_u64, _pow_u64,
 *            _sigma1_sigma2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_divisor_count_u64.h"
#include "../utils/_factor_u64.h"
#include "../utils/_gcd_u64.h"
#include "../utils/_pow_u64.h"
#include "../utils/_sigma1_sigma2.h"

/* ---------- Main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    uint64_t sigma1, sigma2;
    _sigma1_sigma2((uint64_t)n, &sigma1, &sigma2);

    uint64_t g = _gcd_u64(sigma1, sigma2);
    uint64_t dcnt = _divisor_count_u64(g);

    return (seq_t) dcnt;
}
