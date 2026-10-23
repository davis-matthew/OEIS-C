/*
 * A039716  a(n) = prime(n)!.
 * https://oeis.org/A039716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = prime(n)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; get_prime returns the n-th prime (uint32_t). */
    uint32_t p = get_prime(n);
    /* Convert to idx_t and compute factorial. */
    return factorial((idx_t)p);
}
