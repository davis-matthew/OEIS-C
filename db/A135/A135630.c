/*
 * A135630  2^(prime(n) - 2) - 1.
 * https://oeis.org/A135630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(prime(n)-2) - 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; get_prime(1) == 2 */
    uint32_t p = get_prime(n);          /* n‑th prime */
    idx_t exp = (idx_t)p - 2;           /* exponent */
    seq_t val = pow2(exp);              /* 2^exp */
    val -= (seq_t)1;                    /* subtract 1 */
    return val;
}
