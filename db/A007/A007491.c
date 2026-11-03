/*
 * A007491  Smallest prime > n^2.
 * https://oeis.org/A007491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = smallest prime > n^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9499 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    seq_t n_squared = (seq_t)n * (seq_t)n;
    seq_t candidate = n_squared + 1;
    
    while (!is_prime_u64((uint64_t)candidate)) {
        candidate++;
    }
    
    return candidate;
}
