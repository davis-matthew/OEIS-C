/*
 * A182047  Smallest prime > n*(n+1).
 * https://oeis.org/A182047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime > n*(n+1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* compute n*(n+1) in the full precision type seq_t */
    seq_t t = (seq_t)n * ((seq_t)n + 1);

    /* next_prime expects a uint64_t, the value fits comfortably */
    uint64_t p = next_prime((uint64_t)t);

    return (seq_t)p;
}
