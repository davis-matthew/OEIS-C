/*
 * A120804  Primes with consecutive digits descending.
 * https://oeis.org/A120804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose decimal representation is a *
 *            decreasing cyclic sequence of digits: d, d-1, d-2, … (mod 10), *
 *            where the starting digit d is in 1…9 and the length is at least
 *            1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t len = 1; ; ++len) {            /* increasing length guarantees ascending values */
        for (unsigned int d = 1; d <= 9; ++d) { /* start digit cannot be 0 */
            seq_t val = 0;
            unsigned int cur = d;
            for (idx_t i = 0; i < len; ++i) {
                val = val * 10 + cur;
                cur = (cur == 0) ? 9 : cur - 1; /* wrap after 0 */
            }
            if (is_prime_big(val)) {
                ++count;
                if (count == n) {
                    return val;
                }
            }
        }
    }
}
