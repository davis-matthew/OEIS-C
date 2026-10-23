/*
 * A130614  a(n) = p^(p-2), where p = prime(n).
 * https://oeis.org/A130614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p^(p-2), where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {          /* Offset is 1, so n=0 is outside the domain */
        return 0;          /* Return 0 for convenience (undefined case) */
    }

    uint32_t p_u32 = get_prime(n);   /* get_prime(n) returns the n-th prime */
    seq_t p = (seq_t)p_u32;          /* Promote to seq_t */
    seq_t exp = p - (seq_t)2;        /* exponent = p-2 */

    return big_pow(p, exp);          /* compute p^(p-2) in seq_t */
}
