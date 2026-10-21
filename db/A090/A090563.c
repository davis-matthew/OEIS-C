/*
 * A090563  Numbers k such that 5*k^2 + 5*k + 1 is prime.
 * https://oeis.org/A090563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 56, u64 n <= 2468,
 *            u128 n <= 2468, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090563: a(n) = nth positive integer k such that 5*k^2 + 5*k + 1 is prime */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t kv = (seq_t)k;
        seq_t val = (seq_t)5 * kv * kv + (seq_t)5 * kv + (seq_t)1;
        if (is_prime_big(val)) {
            if (++count == n) {
                return kv;
            }
        }
    }
}
