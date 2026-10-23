/*
 * A109925  Number of primes of the form n - 2^k.
 * https://oeis.org/A109925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes of the form n - 2^k (k ≥ 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(uint64_t n) */

seq_t a(idx_t n) {
    seq_t cnt = 0;
    /* Iterate over powers of two: 1, 2, 4, ... up to n-1 */
    for (uint64_t pow = 1; pow <= n - 1; pow <<= 1) {
        uint64_t cand = n - pow;
        if (cand >= 2 && is_prime(cand))
            ++cnt;
        /* safety guard against overflow when n is very large */
        if (pow > UINT64_MAX >> 1)
            break;
    }
    return cnt;
}
