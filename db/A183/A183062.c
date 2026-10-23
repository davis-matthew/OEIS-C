/*
 * A183062  Numbers with 59 divisors.
 * https://oeis.org/A183062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^58
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 2, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The n-th term is the n-th prime raised to the 58th power. */
    uint32_t p = get_prime(n);          /* obtain the n-th prime (uint32_t) */
    return big_pow((seq_t)p, (seq_t)58);
}
