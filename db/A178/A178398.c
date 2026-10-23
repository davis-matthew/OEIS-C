/*
 * A178398  a(n) = p(p+1)^2, where p is the n-th prime.
 * https://oeis.org/A178398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(p+1)^2 where p is the n-th prime
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to uint32_t prime and then to seq_t for computation */
    uint32_t p = get_prime(n);
    seq_t pp = (seq_t)p;
    seq_t inc = pp + 1;
    seq_t sq = inc * inc;
    return pp * sq;
}
