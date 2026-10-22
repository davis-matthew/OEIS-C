/*
 * A007917  Version 1 of the "previous prime" function: largest prime <= n.
 * https://oeis.org/A007917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   largest prime <= n, computed by checking down from n until a
 *            prime is found
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    
    while (val >= 2) {
        if (is_prime((uint64_t)val)) {
            return val;
        }
        val--;
    }
    
    return 2;
}
