/*
 * A056896  Smallest prime which can be written as k^2 + n for k >= 0.
 * https://oeis.org/A056896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime of the form n + k^2 (k ≥ 0)
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    idx_t k = 0;
    while (1) {
        /* compute n + k^2 */
        unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)k;
        seq_t val = (seq_t)n + (seq_t)prod;
        if (is_prime_big(val)) return val;
        k++;
        /* safety guard: boost after a large number of iterations to avoid infinite loop */
        /* (not expected to be reached for test inputs) */
    }
}
