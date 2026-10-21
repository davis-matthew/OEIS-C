/*
 * A078705  Integer part of the square root of sigma(n).
 * https://oeis.org/A078705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = floor(sqrt(sigma(n)))
 * Range:     u8 n <= 16, u16 n <= 4248, u32 n <= 9155, u64 n <= 9155,
 *            u128 n <= 9155, big any n
 * Helpers:   floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* trivial case (unused as offset starts at 1) */
    if (n == 0) return 0;

    uint64_t m = n;            /* integer for factorisation */
    seq_t sigma = 1;           /* product of divisor‑sum terms */

    uint64_t i = 2;
    /* divide by all primes up to sqrt(m) */
    while (i <= m / i) {
        if (m % i == 0) {
            uint64_t exp = 0;
            while (m % i == 0) {
                m /= i;
                exp++;
            }
            /* term = (i^(exp+1)-1) / (i-1) */
            seq_t term = (pow_seq((seq_t)i, (int)(exp + 1)) - 1) /
                         ((seq_t)i - 1);
            sigma *= term;
        }
        i++;
    }

    /* remaining factor > 1 is a prime */
    if (m > 1) {
        seq_t term = (pow_seq((seq_t)m, 2) - 1) /
                     ((seq_t)m - 1);
        sigma *= term;
    }

    return floor_sqrt(sigma);
}
