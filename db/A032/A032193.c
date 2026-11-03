/*
 * A032193  Number of necklaces with 8 black beads and n-8 white beads.
 * https://oeis.org/A032193
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Sum_{d|gcd(n,8)} phi(d) * binom(n/d, 8/d)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 40 (2026-11-01)
 * Helpers:   binomial, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;   /* Undefined below the offset */

    static const idx_t divisors[] = {1, 2, 4, 8};
    const size_t divc = sizeof(divisors) / sizeof(divisors[0]);

    seq_t sum = 0;
    for (size_t i = 0; i < divc; ++i) {
        idx_t d = divisors[i];
        if (n % d == 0) {
            seq_t phi_d = totient(d);
            seq_t bin = binomial(n / d, 8 / d);
            sum += phi_d * bin;
        }
    }

    seq_t N = (seq_t)n;
    return sum / N;
}
