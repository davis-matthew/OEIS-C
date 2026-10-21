/*
 * A084755  Let the sequence s_n be defined by s_n(1) = n+1 and for k > 1,
 *          s_n(k) = k*s_n(k-1)+1. Then a(n) is the first prime in the
 *          sequence s_n.
 * https://oeis.org/A084755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first prime in the sequence * s_n(1) = n+1 * s_n(k) =
 *            k*s_n(k-1) + 1 (k > 1)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* start with s_n(1) = n+1 */
    seq_t val = (seq_t)n + 1;

    /* test the first term */
    if (is_prime_big(val))
        return val;

    /* iterate k = 2, 3, ... until the first prime is found */
    for (idx_t k = 2;; ++k) {
        val = (seq_t)k * val + 1;
        if (is_prime_big(val))
            return val;
    }
}
