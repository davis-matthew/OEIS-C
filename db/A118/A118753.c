/*
 * A118753  First prime after 4n. Smallest prime >= 4*n. Bisection of A060264.
 * https://oeis.org/A118753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime ≥ 4*n (first prime after 4n)
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t target = (seq_t)4 * n;
    seq_t p = target;

    /* Ensure we start at least at 2 */
    if (p < 2) {
        p = 2;
    }

    /* Find the first prime ≥ target */
    while (!is_prime_big(p)) {
        ++p;
    }
    return p;
}
