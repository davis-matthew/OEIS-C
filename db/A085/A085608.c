/*
 * A085608  Index of the smallest prime starting with n.
 * https://oeis.org/A085608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = PrimePi(A018800(n))
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   compute_pi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    uint64_t prefix = (uint64_t)n;
    uint64_t pow10  = 1;

    while (1) {
        /* Overflow guard for the product prefix*pow10. */
        if (prefix > UINT64_MAX / pow10)
            break;

        uint64_t lower = prefix * pow10;
        if (prefix + 1 > UINT64_MAX / pow10)
            break;
        uint64_t upper = (prefix + 1) * pow10 - 1;

        for (uint64_t cand = lower; cand <= upper; ++cand) {
            if (is_prime_big((seq_t)cand)) {
                return compute_pi(cand);
            }
        }

        /* Next block with an extra zero digit. */
        if (pow10 > UINT64_MAX / 10)
            break;
        pow10 *= 10;
    }

    /* Theoretically unreachable for n >= 1. */
    return (seq_t)0;
}
