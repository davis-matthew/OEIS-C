/*
 * A073310  a(n) is the smallest number k such that 2+k and 2n+k are both
 *          prime.
 * https://oeis.org/A073310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that 2 + k and 2*n + k are both prime
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses is_prime_big(seq_t) to test primality of seq_t values. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 1;
    seq_t two   = (seq_t)2;
    seq_t two_n = (seq_t)2 * (seq_t)n;   /* 2*n */

    for (;; ++k) {
        seq_t kp = (seq_t)k;           /* current candidate k as seq_t */
        if (is_prime_big(two + kp) && is_prime_big(two_n + kp))
            return kp;
    }
}
