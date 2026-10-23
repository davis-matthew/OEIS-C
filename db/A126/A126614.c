/*
 * A126614  a(n) = (2^prime(n) + 1)/3.
 * https://oeis.org/A126614
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^prime(n) + 1)/3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2u)          /* behavior below offset is irrelevant */
        return 0;

    /* prime(n) returns the n-th prime (1‑based) as a 32‑bit unsigned integer */
    uint32_t p = get_prime(n);

    /* compute 2^p, add 1, and divide by 3 */
    seq_t result = pow2((idx_t)p);
    result = result + 1;
    result = result / 3;

    return result;
}
