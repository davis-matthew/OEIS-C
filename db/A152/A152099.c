/*
 * A152099  a(n) = (2^prime(n) - 1)*(2^prime(n) + 1) = 2^(2*prime(n)) - 1.
 * https://oeis.org/A152099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*prime(n)) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1, safe for internal callers */
    uint32_t p = get_prime(n);        /* nth prime (1‑based) */
    idx_t exp = 2 * (idx_t)p;         /* exponent = 2*prime(n) */
    seq_t val = pow_uint(2, exp);     /* compute 2^(2*prime(n)) */
    return val - 1;                   /* subtract 1 */
}
