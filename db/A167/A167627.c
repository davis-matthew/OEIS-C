/*
 * A167627  Prime numbers ending in the prime number 163.
 * https://oeis.org/A167627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 18, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  a(n) is the n-th prime whose decimal representation ends in 163
   (i.e. p ≡ 163 (mod 1000)).  a(1) = 163, a(2) = 1163, a(3) = 3163, … */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, not used for n = 0 */

    idx_t count = 0;
    idx_t q = 0;
    seq_t val;

    while (count < n) {
        val = ((seq_t)1000 * q) + 163;
        if (is_prime_big(val)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        q++;
    }
    return 0;  /* unreachable, keeps the compiler happy */
}
