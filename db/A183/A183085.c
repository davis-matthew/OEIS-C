/*
 * A183085  Numbers with 61 divisors.
 * https://oeis.org/A183085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n)^60
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 2, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1, return 0 */

    uint32_t p = get_prime(n);   /* p is the n-th prime (1‑based) */
    seq_t base = (seq_t)p;       /* cast to seq_t (native type) */
    seq_t exp  = (seq_t)60;      /* exponent 60 */

    return big_pow(base, exp);
}
