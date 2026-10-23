/*
 * A139293  (2^(2p - 1)/8)-1, where p is prime.
 * https://oeis.org/A139293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (2^(2p - 1) / 8) - 1, where p is the nth prime.
 * Simplified: a(n) = 2^(2p - 4) - 1
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1; for n = 0 return 0 (undefined). */
        return 0;
    }
    /* Retrieve the nth prime. */
    uint32_t p = get_prime(n);
    idx_t exp = 2 * (idx_t)p - 4;
    seq_t val = pow2(exp);
    val -= 1;          /* subtract 1 to get the sequence value */
    return val;
}
