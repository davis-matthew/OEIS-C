/*
 * A117131  Remainder when n^n is divided by the n-th prime number.
 * https://oeis.org/A117131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n^n mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 6634, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* Get the n-th prime; it fits in uint32_t for n ≤ 10⁴. */
    uint32_t p = get_prime(n);
    /* Compute n^n (mod prime(n)). */
    return mod_pow((seq_t)n, (uint64_t)n, (seq_t)p);
}
