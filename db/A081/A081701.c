/*
 * A081701  a(n) = prime(n) * (prime(n) - 1)^(prime(n) - 1).
 * https://oeis.org/A081701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * (prime(n) - 1)^(prime(n) - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;             /* undefined for n < 1 */
    uint32_t p_uint = get_prime(n);   /* nth prime */
    seq_t p   = (seq_t)p_uint;        /* convert to seq_t */
    seq_t base = p - (seq_t)1;        /* (prime(n) - 1) */
    seq_t exp  = base;                /* exponent = prime(n) - 1 */
    seq_t pow  = big_pow(base, exp);  /* (prime(n)-1)^(prime(n)-1) */
    return p * pow;                   /* final result */
}
