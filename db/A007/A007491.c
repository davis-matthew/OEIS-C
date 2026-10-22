/*
 * A007491  Smallest prime > n^2.
 * https://oeis.org/A007491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = smallest prime > n^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t n_squared = (seq_t)n * (seq_t)n;
    seq_t candidate = n_squared + 1;
    
    while (!is_prime((uint64_t)candidate)) {
        candidate++;
    }
    
    return candidate;
}
