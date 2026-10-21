/*
 * A071825  a(n) = (number of x with largest prime factor of the form 4k+3
 *          less than or equal to n) minus (number of x with largest prime
 *          factor of the form 4k+1 less than or equal to n).
 * https://oeis.org/A071825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = (# x≤n : largest prime factor of x ≡ 3 (mod 4)) − (# x≤n
 *            : largest prime factor of x ≡ 1 (mod 4))
 * Range:     u8 n <= 4532, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    seq_t count3 = 0;
    seq_t count1 = 0;
    for (idx_t i = 2; i <= n; ++i) {
        /* largest_prime_divisor returns the largest prime divisor of i */
        uint64_t lpf = largest_prime_divisor((uint64_t)i);
        if ((lpf & 3ULL) == 3ULL) {
            ++count3;
        } else if ((lpf & 3ULL) == 1ULL) {
            ++count1;
        }
    }
    long long diff = (long long)count3 - (long long)count1;
    return (seq_t)diff;
}
