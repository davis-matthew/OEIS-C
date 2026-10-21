/*
 * A079142  Numbers divisible by prime integer parts of their square roots.
 * https://oeis.org/A079142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(floor(n/3+1)) * ( prime(floor(n/3+1)) + (n mod 3) )
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   kth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/kth_prime.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* compute k = floor(n/3) + 1 */
    idx_t k = n / 3 + 1;

    /* get the k-th prime */
    uint32_t p_uint = kth_prime(k);
    seq_t p = (seq_t)p_uint;

    /* r = n mod 3 */
    uint32_t r_uint = (uint32_t)(n % 3);
    seq_t r = (seq_t)r_uint;

    return p * (p + r);
}
