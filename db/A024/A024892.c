/*
 * A024892  Numbers k such that 3*k+1 is prime.
 * https://oeis.org/A024892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the nth integer k such that 3*k+1 is prime. */
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n<1, but guard */

    idx_t count = 0;
    idx_t k     = 1;

    for (;; ++k) {
        seq_t cand = (seq_t)(k * 3 + 1);      /* 3*k+1 */
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
