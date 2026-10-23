/*
 * A139572  Numbers with 37 divisors.
 * https://oeis.org/A139572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n)^36
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 5, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* As OEIS defines offset 1, the sequence is valid for n ≥ 1.
       No need to handle n < 1, but we guard against it. */
    if (n < 1) return 0;

    /* Obtain the nth prime; get_prime returns a 32‑bit unsigned integer. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute the 36th power of the prime using the arbitrary precision helper. */
    return big_pow(p, (seq_t)36);
}
