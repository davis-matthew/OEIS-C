/*
 * A108187  Numbers n such that 11*n - 5 is prime.
 * https://oeis.org/A108187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that 11*k - 5 is prime
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"  /* defines bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (count < n) {
        seq_t cand = (seq_t)11 * k - 5;
        if (is_prime((uint64_t)cand)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    return 0;  /* unreachable */
}
