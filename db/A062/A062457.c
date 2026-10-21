/*
 * A062457  a(n) = prime(n)^n.
 * https://oeis.org/A062457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth element of OEIS A062457
 * by raising the nth prime to the power n. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Convert the nth prime and the exponent to seq_t type. */
    seq_t base = (seq_t)get_prime(n);
    seq_t exp  = (seq_t)n;
    /* Compute prime(n)^n. */
    return big_pow(base, exp);
}
